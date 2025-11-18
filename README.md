# CMake functions that determines code pages

```cpp
/*
 * Return ANSI Code Page of current locale set by setlocale().
 */
static unsigned
get_current_codepage(void)
{
	char *locale, *p;
	unsigned cp;

	locale = setlocale(LC_CTYPE, NULL);
	if (locale == NULL)
		return (GetACP());
	if (locale[0] == 'C' && locale[1] == '\0')
		return (CP_C_LOCALE);
	p = strrchr(locale, '.');
	if (p == NULL)
		return (GetACP());
	if ((strcmp(p+1, "utf8") == 0) || (strcmp(p+1, "UTF-8") == 0))
		return CP_UTF8;
	cp = my_atoi(p+1);
	if ((int)cp <= 0)
		return (GetACP());
	return (cp);
}
```

```cpp
/*
 * Return OEM Code Page of current locale set by setlocale().
 */
static unsigned
get_current_oemcp(void)
{
	int i;
	char *locale, *p;
	size_t len;

	locale = setlocale(LC_CTYPE, NULL);
	if (locale == NULL)
		return (GetOEMCP());
	if (locale[0] == 'C' && locale[1] == '\0')
		return (CP_C_LOCALE);

	p = strrchr(locale, '.');
	if (p == NULL)
		return (GetOEMCP());
	len = p - locale;
	for (i = 0; acp_ocp_map[i].acp; i++) {
		if (strncmp(acp_ocp_map[i].locale, locale, len) == 0)
			return (acp_ocp_map[i].ocp);
	}
	return (GetOEMCP());
}
```

# Some results

## Case 1

### Configs

* My office PC
	* 🇺🇸 as device setup region
* 🇺🇸 display language
* 🇰🇷 system locale

### Summary

* OEM `949`
* Current `949`
* Used `949` and failed

### Execution result

```java
D:\GitHub\cmake\build\bin>"D:/GitHub/cmake/build/bin/cmake.exe" -E tar xv "D:\GitHub\CodePageTest\data\archive.tar.gz"
[SUNG] `opt_sconv` is NULL
[SUNG] (ACP) Found codepage '949' from locale 'English_VIR.949'
[SUNG] (OEMCP) No OEM locale match found for 'English_VIR.949', using `GetOEMCP()` '949'
[SUNG] Current code page is '949', OEMCP is '949', Charset is ''
[SUNG] Current code page equals to OEMCP, no conversion
[SUNG] `sconv_default` is NULL
[SUNG] `sconv` is still NULL
[SUNG] (ACP) Found codepage '949' from locale 'English_VIR.949'
[SUNG] `sc` is NULL, using current codepage 949
x archive/
[SUNG] `opt_sconv` is NULL
[SUNG] `sconv` is still NULL
[SUNG] (ACP) Found codepage '949' from locale 'English_VIR.949'
[SUNG] `sc` is NULL, using current codepage 949
[SUNG] (ACP) Found codepage '949' from locale 'English_VIR.949'
[SUNG] `sc` is NULL, using current codepage 949
x
[SUNG] (ACP) Found codepage '949' from locale 'English_VIR.949'
[SUNG] `sc` is NULL, using current codepage 949
[SUNG] (ACP) Found codepage '949' from locale 'English_VIR.949'
[SUNG] `sc` is NULL, using current codepage 949
CMake Error: Problem with archive_write_header(): Invalid empty pathname
[SUNG] (ACP) Found codepage '949' from locale 'English_VIR.949'
[SUNG] `sc` is NULL, using current codepage 949
CMake Error: Current file:
CMake Error: Problem extracting tar: D:\GitHub\CodePageTest\data\archive.tar.gz
```

## Case 2

### Configs

* My office PC
* 🇰🇷 display language
* 🇰🇷 system locale

### Summary

* OEM `949`
* Current `949`
* Used `949` and failed

### Execution result

```java
D:\GitHub\cmake\build\bin>"D:/GitHub/cmake/build/bin/cmake.exe" -E tar xv "D:\GitHub\CodePageTest\data\archive.tar.gz"
[SUNG] `opt_sconv` is NULL
[SUNG] (ACP) Found codepage '949' from locale 'Korean_Korea.949'
[SUNG] (OEMCP) Found OEM locale match: Korean_Korea
[SUNG] Current code page is '949', OEMCP is '949', Charset is ''
[SUNG] Current code page equals to OEMCP, no conversion
[SUNG] `sconv_default` is NULL
[SUNG] `sconv` is still NULL
[SUNG] (ACP) Found codepage '949' from locale 'Korean_Korea.949'
[SUNG] `sc` is NULL, using current codepage 949
x archive/
[SUNG] `opt_sconv` is NULL
[SUNG] `sconv` is still NULL
[SUNG] (ACP) Found codepage '949' from locale 'Korean_Korea.949'
[SUNG] `sc` is NULL, using current codepage 949
[SUNG] (ACP) Found codepage '949' from locale 'Korean_Korea.949'
[SUNG] `sc` is NULL, using current codepage 949
x
[SUNG] (ACP) Found codepage '949' from locale 'Korean_Korea.949'
[SUNG] `sc` is NULL, using current codepage 949
[SUNG] (ACP) Found codepage '949' from locale 'Korean_Korea.949'
[SUNG] `sc` is NULL, using current codepage 949
CMake Error: Problem with archive_write_header(): Invalid empty pathname
[SUNG] (ACP) Found codepage '949' from locale 'Korean_Korea.949'
[SUNG] `sc` is NULL, using current codepage 949
CMake Error: Current file:
CMake Error: Problem extracting tar: D:\GitHub\CodePageTest\data\archive.tar.gz
```

## Case 3

### Configs

* My office PC
* 🇰🇷 display language
* 🇺🇸 system locale

### Summary

* OEM `949`
* Current `1252`
* Used `949` and failed

### Execution result

```java
D:\GitHub\cmake\build\bin>"D:/GitHub/cmake/build/bin/cmake.exe" -E tar xv "D:\GitHub\CodePageTest\data\archive.tar.gz"
[SUNG] `opt_sconv` is NULL
[SUNG] (ACP) Found codepage '1252' from locale 'Korean_Korea.1252'
[SUNG] (OEMCP) Found OEM locale match: 'Korean_Korea', '949'
[SUNG] Current code page is '1252', OEMCP is '949', Charset is ''
[SUNG] OEMCP: CP949
[SUNG] `sconv_default` initialized
[SUNG] `sconv` is set
[SUNG] sc is (949 -> 1252), using 949
[SUNG] sc is (949 -> 1252), using 949
[SUNG] sc is (949 -> 1252), using 949
x archive/
[SUNG] `opt_sconv` is NULL
[SUNG] `sconv` is set
[SUNG] sc is (949 -> 1252), using 949
[SUNG] sc is (949 -> 1252), using 949
[SUNG] sc is (949 -> 1252), using 949
x
CMake Error: Problem with archive_write_header(): Invalid empty pathname
CMake Error: Current file:
CMake Error: Problem extracting tar: D:\GitHub\CodePageTest\data\archive.tar.gz
```

## Case 4

### Configs

* My office PC
* 🇺🇸 display language
* 🇺🇸 system locale

### Summary

* OEM `437`
* Current `1252`
* Used `437` and success

### Execution result

```java
D:\GitHub\cmake\build\bin>"D:/GitHub/cmake/build/bin/cmake.exe" -E tar xv "D:\GitHub\CodePageTest\data\archive.tar.gz"
[SUNG] `opt_sconv` is NULL
[SUNG] (ACP) Found codepage '1252' from locale 'English_world.1252'
[SUNG] (OEMCP) No OEM locale match found for 'English_world.1252', using `GetOEMCP()` '437'
[SUNG] Current code page is '1252', OEMCP is '437', Charset is ''
[SUNG] OEMCP: CP437
[SUNG] `sconv_default` initialized
[SUNG] `sconv` is set
[SUNG] sc is (437 -> 1252), using 437
[SUNG] sc is (437 -> 1252), using 437
[SUNG] sc is (437 -> 1252), using 437
x archive/
[SUNG] `opt_sconv` is NULL
[SUNG] `sconv` is set
[SUNG] sc is (437 -> 1252), using 437
[SUNG] sc is (437 -> 1252), using 437
[SUNG] sc is (437 -> 1252), using 437
x archive/φÿ╕φÿ╕φÖï.txt
[SUNG] `opt_sconv` is NULL
[SUNG] `sconv` is set
```
