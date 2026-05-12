test_meow_sources = run_command(
    'py',
    meson.project_source_root() / 'tools' / 'meson_glob.py',
    meson.current_source_dir(), '.',
).stdout().strip().split('\n')

test_meow = executable('test_meow',
	sources: test_meow_sources,
	include_directories: [
		include_directories('.'),
		include_directories('../generated'),
	],
	dependencies: [libfx_dep],
	override_options: ['cpp_std=c++latest'],
	name_prefix: '',
)