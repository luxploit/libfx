project('libfx', 'cpp', 'c',
	default_options: [
		'cpp_std=c++latest',
		'c_std=c17',
		'buildtype=release'
	]
)

# Run the git update script before building
run_command('py', 'tools/update_git.py', check : false)

add_project_arguments(
	'/wd4273',
	'/wd4459',
	'/wd4100',
	'/wd4389',
	'/wd5054',
	'/wd4996',
	'/wd4018',
	'/wd4456',
	'/wd4458',
	'/wd4208',
	'/wd4172',
	'/wd4267',
	'/wd4312',
	'/wd4244',
	'/wd4189',
	'/wd4245',
	language: ['cpp', 'c']
)

add_project_arguments(
	'/clang:-masm=intel',
	'/clang:-Wno-pragma-system-header-outside-header',
	'/clang:-fno-delayed-template-parsing',
	'/clang:-Wno-microsoft-include',
	'/clang:-Wno-microsoft-cast',
	'/clang:-Wno-microsoft-string-literal-from-predefined',
	'/clang:-Wno-c++17-extensions',
	'/clang:-Wno-invalid-token-paste',
	'/clang:-Wno-inconsistent-dllimport',
	'/clang:-Wno-c99-designator',
	'/clang:-Wno-unused-function',
	'/clang:-Wno-unused-private-field',
	'/clang:-Wno-missing-braces',
	'/clang:-mavx2',  # AVX2 vector extensions
	language: ['cpp', 'c']
)

add_project_arguments(
	'/permissive',
	'/UUNICODE',
	'/U_UNICODE',
	'/D_MBCS',
	'/D_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR',
	language: ['cpp', 'c']
)

if get_option('buildtype') == 'release'
	add_project_arguments('-DNDEBUG', language: ['cpp', 'c'])
	add_project_link_arguments('/IGNORE:4702', '/LTCG', language: ['cpp', 'c'])
else
	add_project_arguments('-DDEBUG', '-D_DEBUG', language: ['cpp', 'c'])
endif

add_project_link_arguments('/INCREMENTAL:NO', '/SAFESEH:NO', language: ['cpp', 'c'])

subdir('vendor')
subdir('lib')
subdir('tests')
