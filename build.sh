#!/usr/bin/env bash

error_string="$0: ERROR:"
info_string="$0: INFO:"

build_type=""
build_dir=""
clean=false
run=false
compiler=""
linker=""
debugger=""

dir=false
for arg in "$@"; do
	if [[ "$dir" == true ]]; then
		canonicalized=$(realpath --relative-base=. "$arg" 2>/dev/null) || {
			echo "$error_string wrong path: '$arg'"
			exit 1
		}
		build_dir="$canonicalized"
		dir=false
		continue
	fi
	case $arg in
		-d) ;&
		--debug) build_type=Debug;;
		--relwithdebinfo) build_type=RelWithDebInfo;;
		--release) build_type=Release;;
		--minsizerel) build_type=MinSizeRel;;

		--dir) dir=true;;

		-c) ;&
		--clean) clean=true;;
		-r) ;&
		--run) run=true;;

		--gcc) compiler="gcc";;
		--clang) compiler="clang";;

		--bfd) linker="BFD";;
		--lld) linker="LLD";;

		--gdb) run=true; debugger="gdb";;
		--lldb) run=true; debugger="lldb";;

		*) echo "$error_string unknown argument: '$arg'"; exit 1;;
	esac
done
# Check if the user provided --dir without an argument
if [[ "$dir" == true ]]; then
	echo "$error_string --dir requires an argument"
	exit 1
fi

# Check for CMake
if ! command -v cmake 1>/dev/null; then
	echo "$error_string CMake not present in system. Install it and try again."
	exit 1
fi

# Check for CMakeLists existence
if [[ ! -f "CMakeLists.txt" ]]; then
	echo "$error_string CMakeLists.txt not found in the current directory. Make sure you are in the project root directory."
	exit 1
fi

# The default build type is RelWithDebInfo
if [[ -z "$build_type" ]]; then
	build_type=RelWithDebInfo
fi

# Select default CMake build directory if not specified
if [[ -z "$build_dir" ]]; then
	case $build_type in
		Debug) build_dir="build_debug";;
		RelWithDebInfo) build_dir="build";;
		Release) build_dir="build_release";;
		MinSizeRel) build_dir="build_minsizerel";;
		*)
			echo "$error_string invalid build type: '$build_type'"
			exit 1;;
	esac
fi

# Check if build_dir is not specified
if [[ -z "$build_dir" ]]; then
	echo "$error_string build directory not specified"
	exit 1
fi
# Check if build_dir is not a directory
if [[ -e "$build_dir" && ! -d "$build_dir" ]]; then
	echo "$error_string '$build_dir' exists and is not a directory. Remove it and try again."
	exit 1
fi
# Check if build_dir is the current directory or a parent of it
current_dir_absolute=$(realpath .)
build_dir_absolute=$(realpath "$build_dir")
if [[ "$current_dir_absolute" == "$build_dir_absolute"* ]]; then
    echo "$error_string build directory '$build_dir' cannot be the project root directory or any of its direct ancestors."
    exit 1
fi
# Check if the user has sufficient permissions (if the directory exists)
if [[ -e "$build_dir" && ! ( -r "$build_dir" && -w "$build_dir" && -x "$build_dir" ) ]]; then
	echo "$error_string you have insufficient permissions on '$build_dir'"
	exit 1
fi

echo "$info_string using build type '$build_type' and build dir '$build_dir'"

# Clean build_dir
if [[ "$clean" == true ]]; then
	echo "$info_string removing '$build_dir'"
	rm -r "$build_dir" || {
		echo "$error_string cannot remove '$build_dir'. Try removing manually."
	}
fi

# Configure CMake
if [[ ! -d "$build_dir" ]]; then
	# Select the default compiler and linker if not specified
	if [[ -z "$compiler" ]]; then
		compiler="gcc"
		if command -v clang 1>/dev/null; then
			compiler="clang"
		fi
	fi
	if [[ -z "$linker" ]]; then
		linker="BFD"
		if [[ "$compiler" == "clang" ]] && (command -v ld.lld 1>/dev/null || command -v ld64.lld 1>/dev/null); then
			linker="LLD"
		fi
	fi
	
	echo "$info_string using compiler '$compiler' and linker '$linker'"
	cmake -S . -B "$build_dir" -DCMAKE_BUILD_TYPE="$build_type" \
			-DCMAKE_C_COMPILER="$compiler" -DCMAKE_CXX_COMPILER="$compiler" -DCMAKE_LINKER_TYPE="$linker" || {
		echo "$error_string CMake configuration failed."
		exit 1
	}
	echo "$compiler" > "$build_dir"/compiler.txt
	echo "$linker" > "$build_dir"/linker.txt
else
	echo "$info_string build directory '$build_dir' already present, skipping configuration"
	
	# Check if the user provided toolchain different from already used in build_dir
	existing_compiler=$(cat "$build_dir"/compiler.txt 2>/dev/null)
	existing_linker=$(cat "$build_dir"/linker.txt 2>/dev/null)
	
	if [[ -n "$compiler" && -n "$existing_compiler" ]]; then
		if [[ "$existing_compiler" != "$compiler" ]]; then
			echo "$error_string build directory '$build_dir' was configured with a different compiler '$existing_compiler' \
(now provided '$compiler'). Run with --clean to reconfigure."
			exit 1
		fi
	fi
	if [[ -n "$linker" && -n "$existing_linker" ]]; then
		if [[ "$existing_linker" != "$linker" ]]; then
			echo "$error_string build directory '$build_dir' was configured with a different linker '$existing_linker'. \
(now provided '$linker'). Run with --clean to reconfigure."
			exit 1
		fi
	fi
fi

# Build
cmake --build "$build_dir" --parallel || {
	echo "$error_string build failed."
	exit 1
}

# Run if given the option
if [[ "$run" == true ]]; then
	binary="$build_dir"/battleship
	if [[ -z "$debugger" ]]; then "$binary";
	elif [[ "$debugger" == "gdb" ]]; then gdb -ex run "$binary";
	elif [[ "$debugger" == "lldb" ]]; then lldb -o run "$binary";
	else echo "$error_string unknown debugger '$debugger'"; exit 1;
	fi
fi
