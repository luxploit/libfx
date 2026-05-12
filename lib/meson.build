libfx_sources = run_command(
    'py',
    meson.project_source_root() / 'tools' / 'meson_glob.py',
    meson.current_source_dir(), '.',
).stdout().strip().split('\n')

libfx_lib = static_library('libfx',
	sources: libfx_sources,
	include_directories: [
		include_directories('.'),
		include_directories('../generated'),
	],
	dependencies: [
		libtommath_dep, libtomcrypt_dep, lz4_dep
	],
	cpp_args: ['-DLTM_DESC', '-D_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH'],
	override_options: ['cpp_std=c++latest'],
	name_prefix: '',
)

libfx_dep = declare_dependency(
	link_with: libfx_lib,
	include_directories: [
		include_directories('.'),
		include_directories('../generated'),
	],
	dependencies: [

	],
)