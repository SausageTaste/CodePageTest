#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <print>
#include <set>
#include <sstream>
#include <vector>

#include <Windows.h>


namespace {

    namespace fs = std::filesystem;


    struct CodePageEntry {
        const char* name_;
        unsigned cp_;
    };

    const std::vector<CodePageEntry> CODE_PAGES = {
        { "IBM437", 437 },
        { "ASCII", 1252 },
        { "ASMO-708", 708 },
        { "BIG5", 950 },
        { "CHINESE", 936 },
        { "CP367", 1252 },
        { "CP819", 1252 },
        { "CP1025", 21025 },
        { "DOS-720", 720 },
        { "DOS-862", 862 },
        { "EUC-CN", 51936 },
        { "EUC-JP", 51932 },
        { "EUC-KR", 949 },
        { "EUCCN", 51936 },
        { "EUCJP", 51932 },
        { "EUCKR", 949 },
        { "GB18030", 54936 },
        { "GB2312", 936 },
        { "HEBREW", 1255 },
        { "HZ-GB-2312", 52936 },
        { "IBM273", 20273 },
        { "IBM277", 20277 },
        { "IBM278", 20278 },
        { "IBM280", 20280 },
        { "IBM284", 20284 },
        { "IBM285", 20285 },
        { "IBM290", 20290 },
        { "IBM297", 20297 },
        { "IBM367", 1252 },
        { "IBM420", 20420 },
        { "IBM423", 20423 },
        { "IBM424", 20424 },
        { "IBM819", 1252 },
        { "IBM871", 20871 },
        { "IBM880", 20880 },
        { "IBM905", 20905 },
        { "IBM924", 20924 },
        { "ISO-8859-1", 28591 },
        { "ISO-8859-13", 28603 },
        { "ISO-8859-15", 28605 },
        { "ISO-8859-2", 28592 },
        { "ISO-8859-3", 28593 },
        { "ISO-8859-4", 28594 },
        { "ISO-8859-5", 28595 },
        { "ISO-8859-6", 28596 },
        { "ISO-8859-7", 28597 },
        { "ISO-8859-8", 28598 },
        { "ISO-8859-9", 28599 },
        { "ISO8859-1", 28591 },
        { "ISO8859-13", 28603 },
        { "ISO8859-15", 28605 },
        { "ISO8859-2", 28592 },
        { "ISO8859-3", 28593 },
        { "ISO8859-4", 28594 },
        { "ISO8859-5", 28595 },
        { "ISO8859-6", 28596 },
        { "ISO8859-7", 28597 },
        { "ISO8859-8", 28598 },
        { "ISO8859-9", 28599 },
        { "JOHAB", 1361 },
        { "KOI8-R", 20866 },
        { "KOI8-U", 21866 },
        { "KS_C_5601-1987", 949 },
        { "LATIN1", 1252 },
        { "LATIN2", 28592 },
        { "MACINTOSH", 10000 },
        { "SHIFT-JIS", 932 },
        { "SHIFT_JIS", 932 },
        { "SJIS", 932 },
        { "US", 1252 },
        { "US-ASCII", 1252 },
        { "UTF-16", 1200 },
        { "UTF-16BE", 1201 },
        { "UTF-16LE", 1200 },
        { "UTF-8", CP_UTF8 },
        { "X-EUROPA", 29001 },
        { "X-MAC-ARABIC", 10004 },
        { "X-MAC-CE", 10029 },
        { "X-MAC-CHINESEIMP", 10008 },
        { "X-MAC-CHINESETRAD", 10002 },
        { "X-MAC-CROATIAN", 10082 },
        { "X-MAC-CYRILLIC", 10007 },
        { "X-MAC-GREEK", 10006 },
        { "X-MAC-HEBREW", 10005 },
        { "X-MAC-ICELANDIC", 10079 },
        { "X-MAC-JAPANESE", 10001 },
        { "X-MAC-KOREAN", 10003 },
        { "X-MAC-ROMANIAN", 10010 },
        { "X-MAC-THAI", 10021 },
        { "X-MAC-TURKISH", 10081 },
        { "X-MAC-UKRAINIAN", 10017 },
    };


    fs::path find_repo_root() {
        fs::path path = fs::current_path();

        for (int i = 0; i < 10; ++i) {
            if (fs::is_regular_file(path / ".gitignore") and
                fs::is_regular_file(path / ".clang-format")) {
                return path;
            }

            fs::path parent = path.parent_path();
            if (parent == path)
                break;
            path = std::move(parent);
        }

        throw std::runtime_error("Repository root not found");
    }

    std::string read_file(const fs::path& file_path) {
        std::ifstream file(file_path);
        if (!file) {
            throw std::runtime_error(
                "Failed to open file: " + file_path.string()
            );
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::string utf16_to_utf8(const std::wstring_view u16str) {
        if (u16str.empty()) {
            return std::string{};
        }

        const int required_size = WideCharToMultiByte(
            CP_UTF8,
            0,
            u16str.data(),
            static_cast<int>(u16str.size()),
            nullptr,
            0,
            nullptr,
            nullptr
        );

        if (required_size == 0) {
            throw std::runtime_error(
                "WideCharToMultiByte failed to get required size"
            );
        }

        std::string u8str(required_size, '\0');

        const int converted_size = WideCharToMultiByte(
            CP_UTF8,
            0,
            u16str.data(),
            static_cast<int>(u16str.size()),
            u8str.data(),
            required_size,
            nullptr,
            nullptr
        );

        if (converted_size == 0) {
            throw std::runtime_error(
                "WideCharToMultiByte failed to convert string"
            );
        }

        return u8str;
    }

    char* get_last_error_as_str() {
        DWORD dwLastError = GetLastError();
        if (0 == dwLastError)
            return NULL;

        char* szMessageBuffer = NULL;
        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dwLastError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&szMessageBuffer,
            0,
            NULL
        );

        // NOTE : szMessageBuffer must be freed after use
        return szMessageBuffer;
    }

}  // namespace


int main() {
    system("chcp 65001 > nul");

    const fs::path repo_root = ::find_repo_root();
    const fs::path file_path = repo_root / "unicode.txt";
    const std::string file_contents = ::read_file(file_path);

    for (const CodePageEntry& cp : CODE_PAGES) {
        std::array<wchar_t, 1024> dest{ 0 };

        const int result = MultiByteToWideChar(
            cp.cp_,
            MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
            file_contents.data(),
            file_contents.size(),
            dest.data(),
            dest.size()
        );

        if (0 != result) {
            const std::wstring_view u16str(
                reinterpret_cast<const wchar_t*>(dest.data()),
                static_cast<size_t>(result)
            );
            const auto u8str = ::utf16_to_utf8(u16str);
            std::println("{:>5}  {:<20}  O {}", cp.cp_, cp.name_, u8str);
        } else {
            std::print(
                "{:>5}  {:<20}  X {}",
                cp.cp_,
                cp.name_,
                ::get_last_error_as_str()
            );
        }
    }

    return 0;
}
