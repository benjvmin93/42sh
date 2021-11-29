project(
  '42sh',
  'c',
  default_options: [
    'c_std=c99',
    'warning_level=3',
    'werror=true',
    'b_sanitize=address',
    #'default_library=static',
    #'buildtype=debug',
    ],
  )

summary({
  'optimization': get_option('optimization'),
  'debug': get_option('debug'),
  'b_sanitize': get_option('b_sanitize'),
  'warning_level': get_option('warning_level'),
  'werror': get_option('werror'),
  })

cflags= ['-D_GNU_SOURCE', '-D_POSIX_C_SOURCE=200809L']
add_project_arguments(cflags, language: 'c')

#incdirs = [
  #include_directories('src/lexer'),
  #include_directories('src/basic_prompt'),
  #include_directories('src/parser'),
  #include_directories('src/utils'),
  #]

#common = [
  #'src/lexer/lexer.c',
  #'src/lexer/token.c',
  #'src/parser/parser.c',
  #'src/utils/clist.c',
  #]

all_sources = []
subdir('src')

#cc = meson.get_compiler('c')
#readline_dep = cc.find_library('readline', required: true)

bin = executable(
  '42sh',
  all_sources,
  include_directories: 'src',
  #install: true,
  #dependencies: [readline_dep],
  )
