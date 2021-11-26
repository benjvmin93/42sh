project(
  'example',
  'c',
  default_options: [
    'c_std=c99',
    'warning_level=3',
    'werror=true',
  ],
)

cflags= ['-D_GNU_SOURCE', '-D_POSIX_C_SOURCE=200809L']
add_project_arguments(cflags, language: 'c')

incdirs = [
  include_directories('src/lexer'),
  include_directories('src/basic_prompt'),
  #include_directories('src/parser'),
  include_directories('src/utils'),
]

common = [
  'src/lexer/lexer.c',
  'src/lexer/token.c',
  #'src/parser/parser.c',
  'src/utils/clist.c',
]

lexer = executable(
  'lexer',
  common + [ 'src/basic_prompt/main.c' ],
  include_directories: incdirs
)
