#!/usr/bin/env python3
"""
method_pattern_tool.py - CLI tool for analyzing IL2CPP dumps, resolving classes,
auditing PointerFunctions.cpp, and generating method patterns for Nazi Mod.
"""

import sys
import os
import re
import argparse
from pathlib import Path
from typing import List, Dict, Optional, Tuple, Any

# Ensure stdout and stderr support UTF-8 / non-ASCII characters on Windows console
if hasattr(sys.stdout, "reconfigure"):
    sys.stdout.reconfigure(encoding="utf-8", errors="replace")
if hasattr(sys.stderr, "reconfigure"):
    sys.stderr.reconfigure(encoding="utf-8", errors="replace")


# Paths
SCRIPT_DIR = Path(__file__).resolve().parent
PROJECT_DIR = SCRIPT_DIR.parent
DEFAULT_DUMP_DIRS = [
    PROJECT_DIR / "out" / "build" / "x64-Debug-NoFeature" / "dumps",
    PROJECT_DIR / "out" / "build" / "x64-Debug" / "dumps",
    PROJECT_DIR / "out" / "build" / "x64-Release" / "dumps",
    PROJECT_DIR / "dumps",
]
CLASS_FINDER_PATH = PROJECT_DIR / "src" / "runtime" / "game" / "import" / "ClassFinder.cpp"
POINTER_FUNCTIONS_PATH = PROJECT_DIR / "src" / "runtime" / "game" / "import" / "PointerFunctions.cpp"

def find_dump_dir(custom_path: Optional[str] = None) -> Path:
    if custom_path:
        p = Path(custom_path)
        if p.is_dir():
            return p
        raise FileNotFoundError(f"Custom dump directory not found: {custom_path}")
    for p in DEFAULT_DUMP_DIRS:
        if p.is_dir() and any(p.glob("*.dll.cs")):
            return p
    raise FileNotFoundError("Could not locate dumps directory in default build paths.")

class MethodInfo:
    def __init__(self, index: int, offset: str, va: str, sig_pattern: str, decl: str,
                 modifier: str, return_type: str, name: str, params: List[str], is_static: bool):
        self.index = index
        self.offset = offset
        self.va = va
        self.sig_pattern = sig_pattern
        self.decl = decl
        self.modifier = modifier
        self.return_type = return_type
        self.name = name
        self.params = params
        self.is_static = is_static

    def __repr__(self):
        return f"<Method 0x{self.index:x} {self.name}({', '.join(self.params)}) -> {self.return_type}>"

class ClassInfo:
    def __init__(self, name: str, namespace: str, field_count: int, method_count: int,
                 file_path: Path, start_line: int, end_line: int, field_pattern: Optional[str] = None):
        self.name = name
        self.namespace = namespace
        self.field_count = field_count
        self.method_count = method_count
        self.file_path = file_path
        self.start_line = start_line
        self.end_line = end_line
        self.field_pattern = field_pattern
        self.methods: List[MethodInfo] = []

    def __repr__(self):
        return f"<Class {self.namespace}.{self.name} methods={self.method_count} file={self.file_path.name}>"

def parse_class_finder_mappings() -> Dict[str, Dict[str, Any]]:
    """Parse ClassFinder.cpp to find KeyName -> (Namespace, FieldPattern, MethodCount)"""
    if not CLASS_FINDER_PATH.exists():
        return {}

    with open(CLASS_FINDER_PATH, "r", encoding="utf-8") as f:
        content = f.read()

    # 1. Parse FieldPatterns
    field_patterns: Dict[str, List[Optional[str]]] = {}
    fp_regex = re.compile(r'FieldPattern\s+([A-Za-z0-9_]+)\s*\{([^}]+)\};')
    for m in fp_regex.finditer(content):
        name = m.group(1)
        raw_items = m.group(2)
        items = []
        for item in raw_items.split(','):
            item = item.strip()
            if item == 'nullptr':
                items.append(None)
            else:
                m_str = re.search(r'"([^"]*)"', item)
                if m_str:
                    items.append(m_str.group(1))
                else:
                    items.append(item)
        field_patterns[name] = items

    # 2. Parse AddQueue calls
    # AddQueue("KeyName", "Namespace", &FieldPatterns::PatternName[, MethodCount]);
    # AddQueue("KeyName", "Namespace");
    queue_regex = re.compile(
        r'AddQueue\s*\(\s*"([^"]+)"\s*,\s*"([^"]*)"(?:\s*,\s*&FieldPatterns::([A-Za-z0-9_]+))?(?:\s*,\s*([0-9]+))?\s*\);'
    )
    queues: Dict[str, Dict[str, Any]] = {}
    for m in queue_regex.finditer(content):
        key_name = m.group(1)
        namespace = m.group(2)
        pattern_name = m.group(3)
        method_count = int(m.group(4)) if m.group(4) else None
        pattern_items = field_patterns.get(pattern_name) if pattern_name else None

        queues[key_name] = {
            "key": key_name,
            "namespace": namespace,
            "pattern_name": pattern_name,
            "pattern_items": pattern_items,
            "method_count": method_count
        }

    return queues

def match_field_pattern(class_field_pattern: str, target_items: List[Optional[str]]) -> bool:
    """Compare dumped FieldPattern: {...} with ClassFinder target items."""
    # dumped format: {"Int32", "Int32", nullptr, ...}
    clean = class_field_pattern.strip().lstrip('{').rstrip('}').strip()
    tokens = [t.strip() for t in clean.split(',')]
    dump_items: List[Optional[str]] = []
    for t in tokens:
        if not t:
            continue
        if t == 'nullptr':
            dump_items.append(None)
        else:
            m = re.search(r'"([^"]*)"', t)
            dump_items.append(m.group(1) if m else t)

    if len(dump_items) != len(target_items):
        return False

    for d, target in zip(dump_items, target_items):
        if target is None:
            continue
        if d != target:
            return False
    return True

def find_class_in_dumps(dump_dir: Path, class_or_key: str,
                        mappings: Optional[Dict[str, Dict[str, Any]]] = None) -> Optional[ClassInfo]:
    """Find a class across all *.dll.cs dump files, either by key or direct class name."""
    if mappings is None:
        mappings = parse_class_finder_mappings()

    queue_info = mappings.get(class_or_key)
    target_ns = queue_info["namespace"] if queue_info else None
    target_items = queue_info.get("pattern_items") if queue_info else None

    # Search order: Assembly-CSharp.dll.cs first, then others
    all_files = sorted(dump_dir.glob("*.dll.cs"), key=lambda p: (p.name != "Assembly-CSharp.dll.cs", p.name))

    for dump_file in all_files:
        with open(dump_file, "r", encoding="utf-8", errors="ignore") as f:
            lines = f.readlines()

        i = 0
        n = len(lines)
        while i < n:
            line = lines[i]
            if line.startswith("// Namespace:"):
                # Header format: // Namespace: <ns> FieldCount: <fc> MethodCount: <mc>
                m_header = re.search(r'// Namespace:\s*([^\s]*)\s+FieldCount:\s*([0-9]+)\s+MethodCount:\s*([0-9]+)', line)
                if not m_header:
                    i += 1
                    continue
                ns_val = m_header.group(1)
                if ns_val == "none":
                    ns_val = ""
                fc = int(m_header.group(2))
                mc = int(m_header.group(3))

                # Next line is class declaration (skipping attributes like [Serializable])
                class_decl_idx = i + 1
                while class_decl_idx < n:
                    line_clean = lines[class_decl_idx].strip()
                    if not line_clean or line_clean.startswith("["):
                        class_decl_idx += 1
                        continue
                    break
                if class_decl_idx >= n:
                    break

                decl_line = lines[class_decl_idx].strip()
                # e.g.: public class Foo : Bar
                m_class = re.search(r'(?:public|private|internal|protected)?\s*(?:sealed|static|abstract)?\s*(?:class|struct|interface|enum)\s+([A-Za-z0-9_$\u4e00-\u9fa5]+)', decl_line)
                class_name = m_class.group(1) if m_class else ""

                # Look for FieldPattern
                fp_line_idx = class_decl_idx + 1
                field_pattern_str = None
                while fp_line_idx < min(n, class_decl_idx + 10):
                    if "// FieldPattern:" in lines[fp_line_idx]:
                        m_fp = re.search(r'// FieldPattern:\s*(\{[^}]*\})', lines[fp_line_idx])
                        if m_fp:
                            field_pattern_str = m_fp.group(1)
                        break
                    if "// Methods" in lines[fp_line_idx]:
                        break
                    fp_line_idx += 1

                # Check match:
                # 1. Match via ClassFinder queue
                if target_items is not None:
                    if (target_ns == "" or ns_val == target_ns) and field_pattern_str:
                        if match_field_pattern(field_pattern_str, target_items):
                            return extract_class_methods(dump_file, lines, i, class_name, ns_val, fc, mc, field_pattern_str)

                # 2. Match by direct class name
                if class_name == class_or_key:
                    if target_ns is None or ns_val == target_ns or not target_ns:
                        return extract_class_methods(dump_file, lines, i, class_name, ns_val, fc, mc, field_pattern_str)

                # 3. Match by namespace.class
                if f"{ns_val}.{class_name}" == class_or_key:
                    return extract_class_methods(dump_file, lines, i, class_name, ns_val, fc, mc, field_pattern_str)

            i += 1

    return None

def extract_class_methods(file_path: Path, lines: List[str], start_idx: int,
                          class_name: str, namespace: str, fc: int, mc: int,
                          field_pattern: Optional[str]) -> ClassInfo:
    """Parse methods of a matched class until end of class brace."""
    cls = ClassInfo(class_name, namespace, fc, mc, file_path, start_idx, start_idx, field_pattern)

    n = len(lines)
    i = start_idx
    # Find start of class body
    while i < n and '{' not in lines[i]:
        i += 1
    i += 1  # inside class

    # Parse methods
    while i < n:
        line = lines[i]
        # Class ends when next class begins or top-level closing brace
        if line.startswith("// Namespace:") or (line.startswith("}") and not line.startswith("\t}")):
            cls.end_line = i
            break

        if "// SignaturePattern:" in line:
            sig_pattern = line.strip().split("// SignaturePattern:")[1].strip().rstrip(';')
            # Next lines have Index, Offset, VA
            idx_line = lines[i + 1] if i + 1 < n else ""
            decl_line = lines[i + 2] if i + 2 < n else ""

            m_idx = re.search(r'// Index:\s*0x([0-9a-fA-F]+)\s+Offset:\s*0x([0-9a-fA-F]+)\s+VA:\s*0x([0-9a-fA-F]+)', idx_line)
            if m_idx:
                idx_val = int(m_idx.group(1), 16)
                offset_val = "0x" + m_idx.group(2)
                va_val = "0x" + m_idx.group(3)
            else:
                idx_val = -1
                offset_val = ""
                va_val = ""

            # Parse C# signature
            decl_str = decl_line.strip()
            # Parse components from sig_pattern:
            # {"modifier", "returnType", "name" or nullptr, {"param1", "param2"}}
            m_sig = re.search(r'\{\s*"([^"]*)"\s*,\s*(?:"([^"]*)"|nullptr)\s*,\s*(?:"([^"]*)"|nullptr)\s*,\s*\{([^}]*)\}\s*\}', sig_pattern)
            if m_sig:
                modifier = m_sig.group(1)
                ret_type = m_sig.group(2) or "Void"
                method_name = m_sig.group(3) or ""
                raw_params = m_sig.group(4).strip()
                params = [p.strip().strip('"') for p in raw_params.split(',') if p.strip()]
            else:
                modifier = ""
                ret_type = "Void"
                method_name = ""
                params = []

            # If method_name is empty (obfuscated), get from decl_line
            is_static = "static" in modifier

            meth = MethodInfo(
                index=idx_val,
                offset=offset_val,
                va=va_val,
                sig_pattern=sig_pattern,
                decl=decl_str,
                modifier=modifier,
                return_type=ret_type,
                name=method_name,
                params=params,
                is_static=is_static
            )
            cls.methods.append(meth)
            i += 2

        i += 1

    return cls

# Type translation helpers
TYPE_MAP_CS_TO_CPP = {
    "Boolean": "bool",
    "bool": "bool",
    "Int32": "int",
    "int": "int",
    "UInt32": "uint32_t",
    "Int64": "int64_t",
    "long": "int64_t",
    "UInt64": "uint64_t",
    "Single": "float",
    "float": "float",
    "Double": "double",
    "double": "double",
    "Void": "void",
    "void": "void",
    "String": "IL2CPP::String*",
    "string": "IL2CPP::String*",
    "Object": "IL2CPP::Object*",
    "object": "IL2CPP::Object*",
    "Vector3": "Vector3",
    "Quaternion": "Quaternion",
    "Color": "Color",
    "Ray": "Ray",
    "RaycastHit": "RaycastHit*",
    "Byte": "char",
    "byte": "char",
}

def map_cs_type_to_cpp(cs_type: str) -> str:
    if cs_type in TYPE_MAP_CS_TO_CPP:
        return TYPE_MAP_CS_TO_CPP[cs_type]
    if cs_type.endswith("[]"):
        inner = cs_type[:-2]
        return f"IL2CPP::Array<{map_cs_type_to_cpp(inner)}>*"
    if cs_type.startswith("List`"):
        return "IL2CPP::List<IL2CPP::Object*>*"
    if cs_type.startswith("Dictionary`"):
        return "IL2CPP::Object*"
    if cs_type == "ENUM":
        return "int"
    return "IL2CPP::Object*"

# Command implementations
def cmd_resolve_class(args, dump_dir: Path):
    mappings = parse_class_finder_mappings()
    target = args.class_name
    cls = find_class_in_dumps(dump_dir, target, mappings)
    if not cls:
        print(f"[ERROR] Class '{target}' not found in dumps.")
        return 1

    print("=" * 60)
    print(f"Target Key / Query : {target}")
    if target in mappings:
        m = mappings[target]
        print(f"ClassFinder Queue  : Namespace='{m['namespace']}' MethodCount={m['method_count']}")
        print(f"FieldPattern Target: {m['pattern_name']} -> {m['pattern_items']}")
    print(f"Dump File          : {cls.file_path.name}")
    print(f"Dumped Class Name  : {cls.name}")
    print(f"Namespace          : {cls.namespace if cls.namespace else '(none)'}")
    print(f"Field Count        : {cls.field_count}")
    print(f"Method Count       : {cls.method_count} (Parsed {len(cls.methods)} methods)")
    if cls.field_pattern:
        print(f"Field Pattern      : {cls.field_pattern}")
    print("=" * 60)
    return 0

def cmd_inspect_methods(args, dump_dir: Path):
    mappings = parse_class_finder_mappings()
    target = args.class_name
    cls = find_class_in_dumps(dump_dir, target, mappings)
    if not cls:
        print(f"[ERROR] Class '{target}' not found in dumps.")
        return 1

    print(f"Class: {cls.namespace}.{cls.name} (Methods: {len(cls.methods)})")
    filtered = cls.methods

    if args.index is not None:
        idx_val = int(args.index, 16) if args.index.startswith("0x") else int(args.index)
        if getattr(args, "window", None) is not None:
            w = args.window
            filtered = [m for m in filtered if idx_val - w <= m.index <= idx_val + w]
        else:
            filtered = [m for m in filtered if m.index == idx_val]

    if args.filter:
        flt = args.filter.lower()
        filtered = [
            m for m in filtered
            if flt in m.name.lower() or flt in m.decl.lower() or flt in m.return_type.lower()
            or any(flt in p.lower() for p in m.params)
        ]

    if not filtered:
        print("No matching methods found.")
        return 0

    limit = args.limit or len(filtered)
    for m in filtered[:limit]:
        print("-" * 50)
        print(f"Index: 0x{m.index:x} ({m.index}) | Offset: {m.offset} | VA: {m.va}")
        print(f"SigPattern: {m.sig_pattern}")
        print(f"C# Decl   : {m.decl}")

    if len(filtered) > limit:
        print(f"... ({len(filtered) - limit} more methods truncated. Use --limit to show more)")
    return 0

def cmd_match_signature(args, dump_dir: Path):
    mappings = parse_class_finder_mappings()
    cls = find_class_in_dumps(dump_dir, args.class_name, mappings)
    if not cls:
        print(f"[ERROR] Class '{args.class_name}' not found.")
        return 1

    matches = []
    target_mod = args.modifier
    target_ret = args.return_type
    target_name = args.name
    target_params = [p.strip() for p in args.params.split(',')] if args.params else None

    for m in cls.methods:
        if target_mod and target_mod != "*" and m.modifier != target_mod:
            continue
        if target_ret and target_ret != "*" and m.return_type != target_ret:
            continue
        if target_name and target_name != "*" and target_name != "nullptr" and m.name != target_name:
            continue
        if target_params is not None:
            if len(target_params) != len(m.params):
                continue
            param_match = True
            for tp, mp in zip(target_params, m.params):
                if tp == "*" or tp == "nullptr":
                    continue
                if tp != mp:
                    param_match = False
                    break
            if not param_match:
                continue
        matches.append(m)

    print(f"Signature search in '{cls.name}': {len(matches)} match(es)")
    for m in matches:
        print(f"  [0x{m.index:x}] {m.decl}")
        print(f"         SigPattern: {m.sig_pattern}")
    return 0

def cmd_gen_pointer(args, dump_dir: Path):
    mappings = parse_class_finder_mappings()
    cls = find_class_in_dumps(dump_dir, args.class_name, mappings)
    if not cls:
        print(f"[ERROR] Class '{args.class_name}' not found.")
        return 1

    target_method = None
    if args.method.startswith("0x") or args.method.isdigit():
        idx_val = int(args.method, 16) if args.method.startswith("0x") else int(args.method)
        for m in cls.methods:
            if m.index == idx_val:
                target_method = m
                break
    else:
        for m in cls.methods:
            if m.name == args.method:
                target_method = m
                break

    if not target_method:
        print(f"[ERROR] Method '{args.method}' not found in class '{cls.name}'.")
        return 1

    # Build C++ Pointer signature
    cpp_ret = map_cs_type_to_cpp(target_method.return_type)
    cpp_args = []
    if not target_method.is_static:
        cpp_args.append("IL2CPP::Object* instance")

    for idx, p in enumerate(target_method.params):
        cpp_type = map_cs_type_to_cpp(p)
        cpp_args.append(f"{cpp_type} arg{idx+1}")

    args_str = ", ".join(cpp_args)
    ptr_name = args.name or (target_method.name if target_method.name else "MethodName")
    class_arg = f'"{args.class_name}"'

    # Output formatted Pointer declaration
    code = f"""\tPointer<{cpp_ret}({args_str})> {ptr_name}(
\t\t{class_arg},
\t\t0x{target_method.index:x}
\t);"""

    print("// Generated C++ Pointer definition:")
    print(code)
    print("\n// Alternative with SignaturePattern:")
    print(f"""\tPointer<{cpp_ret}({args_str})> {ptr_name}(
\t\t{class_arg},
\t\t{target_method.sig_pattern}
\t);""")
    return 0

def cmd_audit(args, dump_dir: Path):
    """Audit PointerFunctions.cpp against dumped classes."""
    if not POINTER_FUNCTIONS_PATH.exists():
        print(f"[ERROR] {POINTER_FUNCTIONS_PATH} not found.")
        return 1

    mappings = parse_class_finder_mappings()
    with open(POINTER_FUNCTIONS_PATH, "r", encoding="utf-8") as f:
        cpp_code = f.read()

    # Regex to find Pointer<ReturnType(Args)> Name("ClassName", Target[, offset]);
    ptr_regex = re.compile(
        r'Pointer<((?:[^<>]|<(?:[^<>]|<[^<>]*>)*>)+)>\s+([A-Za-z0-9_]+)\s*\(\s*"([^"]+)"\s*,\s*([^;]+?)\s*\);',
        re.DOTALL
    )

    total = 0
    ok_count = 0
    issues: List[Tuple[str, str, str, str]] = []

    classes_cache: Dict[str, Optional[ClassInfo]] = {}

    for m in ptr_regex.finditer(cpp_code):
        sig = m.group(1).strip()
        var_name = m.group(2).strip()
        class_key = m.group(3).strip()
        target_expr = m.group(4).strip()

        if args.class_name and class_key != args.class_name:
            continue

        total += 1

        # Cache class resolution
        if class_key not in classes_cache:
            classes_cache[class_key] = find_class_in_dumps(dump_dir, class_key, mappings)

        cls = classes_cache[class_key]
        if not cls:
            issues.append((var_name, class_key, "[MISSING_CLASS]", f"Class '{class_key}' cannot be resolved in dumps."))
            continue

        # Strategy 1: Index-based lookup
        # e.g., 0x58 or 21
        m_index = re.match(r'^(0x[0-9a-fA-F]+|[0-9]+)$', target_expr)
        if m_index:
            idx = int(m_index.group(1), 16) if m_index.group(1).startswith("0x") else int(m_index.group(1))
            if idx >= cls.method_count or idx < 0:
                issues.append((var_name, class_key, "[INDEX_OUT_OF_BOUNDS]",
                               f"Index 0x{idx:x} out of bounds (MethodCount={cls.method_count})"))
                continue

            # Find method at index
            target_method = None
            for meth in cls.methods:
                if meth.index == idx:
                    target_method = meth
                    break

            if not target_method:
                issues.append((var_name, class_key, "[INDEX_NOT_FOUND]",
                               f"No dumped method found at index 0x{idx:x}"))
                continue

            # Compare parameters count
            # Parse C++ params
            m_fn = re.match(r'^[^(]+\((.*)\)$', sig)
            cpp_params = [p.strip() for p in m_fn.group(1).split(',')] if m_fn and m_fn.group(1).strip() else []
            # Deduct instance parameter if non-static
            expected_cs_param_count = len(cpp_params) - (0 if target_method.is_static else 1)
            actual_cs_param_count = len(target_method.params)

            if expected_cs_param_count != actual_cs_param_count:
                # Search nearby indices for shifted candidate
                shifted_candidates = []
                for offset in range(-10, 11):
                    cand_idx = idx + offset
                    if cand_idx == idx or cand_idx < 0 or cand_idx >= cls.method_count:
                        continue
                    cand_meth = next((x for x in cls.methods if x.index == cand_idx), None)
                    if cand_meth:
                        cand_cs_count = len(cand_meth.params)
                        if cand_cs_count == expected_cs_param_count:
                            shifted_candidates.append(cand_meth)

                cand_str = ""
                if shifted_candidates:
                    cand_str = " | Suggested candidates: " + ", ".join(f"0x{c.index:x} ({c.decl})" for c in shifted_candidates[:3])

                issues.append((var_name, class_key, "[PARAM_COUNT_MISMATCH]",
                               f"Index 0x{idx:x} method has {actual_cs_param_count} params, but C++ expects {expected_cs_param_count}{cand_str}"))
                continue

            ok_count += 1
            continue

        # Strategy 2: Name-based lookup
        # e.g., "get_isFocused"
        m_name = re.match(r'^"([^"]+)"(?:\s*,\s*(-?[0-9]+))?$', target_expr)
        if m_name:
            target_name = m_name.group(1)
            offset = int(m_name.group(2)) if m_name.group(2) else 0

            found_method = None
            for meth in cls.methods:
                if meth.name == target_name:
                    found_method = meth
                    break

            if not found_method:
                issues.append((var_name, class_key, "[NAME_NOT_FOUND]",
                               f"Method '{target_name}' not found by name in '{cls.name}' (may be obfuscated)"))
                continue

            ok_count += 1
            continue

        # Strategy 3: Signature pattern
        # e.g., { "internal", "Void", ... }
        if "{" in target_expr:
            ok_count += 1
            continue

        ok_count += 1

    print("=" * 70)
    print(f"POINTER AUDIT RESULTS: {ok_count}/{total} Valid")
    print("=" * 70)

    if issues:
        print(f"\n[!] Found {len(issues)} potential issue(s):")
        for var_name, class_key, status, details in issues:
            print(f"  {status:22} {class_key}::{var_name}")
            print(f"      -> {details}")
    else:
        print("\nAll audited pointers are valid!")

    return 0 if not issues else 1

def main():
    parser = argparse.ArgumentParser(description="Nazi Mod IL2CPP Method Pattern Tool")
    parser.add_argument("--dump-dir", help="Path to dumps directory (default: auto-detect)")
    subparsers = parser.add_subparsers(dest="command", required=True)

    # resolve-class
    p_resolve = subparsers.add_parser("resolve-class", help="Resolve a class across all dump files")
    p_resolve.add_argument("class_name", help="Class name or ClassFinder queue key")

    # inspect-methods
    p_inspect = subparsers.add_parser("inspect-methods", help="List and inspect methods of a class")
    p_inspect.add_argument("class_name", help="Class name or ClassFinder queue key")
    p_inspect.add_argument("--index", help="Inspect specific index (e.g. 0x15)")
    p_inspect.add_argument("--window", type=int, help="Show window of methods around --index (e.g. 3)")
    p_inspect.add_argument("--filter", help="Filter by name, return type, or parameter")
    p_inspect.add_argument("--limit", type=int, default=25, help="Limit number of output methods")

    # match-signature
    p_match = subparsers.add_parser("match-signature", help="Find methods matching a signature pattern")
    p_match.add_argument("class_name", help="Class name or ClassFinder queue key")
    p_match.add_argument("--modifier", default="*", help="Access modifier (e.g. public, internal static, *)")
    p_match.add_argument("--return-type", default="*", help="Return type (e.g. Boolean, Int32, *)")
    p_match.add_argument("--name", default="*", help="Method name or nullptr")
    p_match.add_argument("--params", help="Comma-separated parameter types (e.g. 'Int32,Boolean')")

    # gen-pointer
    p_gen = subparsers.add_parser("gen-pointer", help="Generate C++ Pointer declaration for a method")
    p_gen.add_argument("class_name", help="Class name or ClassFinder queue key")
    p_gen.add_argument("method", help="Method index (0x...) or exact name")
    p_gen.add_argument("--name", help="C++ pointer variable name (optional)")

    # audit
    p_audit = subparsers.add_parser("audit", help="Audit PointerFunctions.cpp against current dumps")
    p_audit.add_argument("--class-name", help="Audit pointers for a specific class only")

    args = parser.parse_args()

    try:
        dump_dir = find_dump_dir(args.dump_dir)
    except FileNotFoundError as e:
        print(f"[ERROR] {e}")
        return 1

    if args.command == "resolve-class":
        return cmd_resolve_class(args, dump_dir)
    elif args.command == "inspect-methods":
        return cmd_inspect_methods(args, dump_dir)
    elif args.command == "match-signature":
        return cmd_match_signature(args, dump_dir)
    elif args.command == "gen-pointer":
        return cmd_gen_pointer(args, dump_dir)
    elif args.command == "audit":
        return cmd_audit(args, dump_dir)
    return 0

if __name__ == "__main__":
    sys.exit(main())
