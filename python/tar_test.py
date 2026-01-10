import os
import shutil
import pathlib


def __remove_all_extracted_folders():
    for x in os.listdir():
        if not os.path.isdir(x):
            continue
        if not x.startswith("test-"):
            continue
        shutil.rmtree(x)


def __try_renaming(original: str, new_name: str):
    if os.path.isdir(original) or os.path.isfile(original):
        os.rename(original, new_name)


def main():
    __remove_all_extracted_folders()

    repo_root_dir = pathlib.Path(__file__).parent.parent.resolve()
    test_tar_path = repo_root_dir / "data" / "utf-8-paths.tar.gz"
    custom_cmake_path = repo_root_dir / "build" / "submodules" / "cmake" / "bin" / "cmake.exe"

    print("-" * 80)
    os.system("cmake --version")
    result = os.system(f"cmake -E tar xvf \"{test_tar_path}\"")
    __try_renaming("utf-8-paths", "test-utf-8-paths-global-cmake")
    print(f"Result: {result}")

    print("-" * 80)
    os.system("tar --version")
    result = os.system(f"tar -xvf \"{test_tar_path}\"")
    __try_renaming("utf-8-paths", "test-utf-8-paths-global-tar")
    print(f"Result: {result}")

    print("-" * 80)
    os.system(f"{custom_cmake_path} --version")
    result = os.system(f"{custom_cmake_path} -E tar xvf \"{test_tar_path}\"")
    __try_renaming("utf-8-paths", "test-utf-8-paths-custom-cmake")
    print(f"Result: {result}")


if __name__ == "__main__":
    main()
