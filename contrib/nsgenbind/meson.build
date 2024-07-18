lexer = find_program('flex')
parser = find_program('bison')

nsgenbind_lexer = custom_target('Nsgenbind lexer',
    input: 'src/nsgenbind-lexer.l',
    output: ['nsgenbind-lexer.c', 'nsgenbind-lexer.h'],
    command: [lexer, '--outfile=@OUTPUT0@', '--header-file=@OUTPUT1@', '@INPUT@']
)

nsgenbind_parser = custom_target('Nsgenbind parser',
    input: 'src/nsgenbind-parser.y',
    output: ['nsgenbind-parser.c', 'nsgenbind-parser.h'],
    command: [parser, '-d', '-p', 'nsgenbind_', '-o', '@OUTPUT0@', '@INPUT@']
)

webidl_lexer = custom_target('Webidl lexer',
    input: 'src/webidl-lexer.l',
    output: ['webidl-lexer.c', 'webidl-lexer.h'],
    command: [lexer, '--outfile=@OUTPUT0@', '--header-file=@OUTPUT1@', '@INPUT@']
)

webidl_parser = custom_target('Webidl parser',
    input: 'src/webidl-parser.y',
    output: ['webidl-parser.c', 'webidl-parser.h'],
    command: [parser, '-d', '-p', 'webidl_', '-o', '@OUTPUT0@', '@INPUT@']
)

nsgenbind_src = files(
    'src/nsgenbind.c',
	'src/utils.c',
	'src/output.c',
	'src/webidl-ast.c',
	'src/nsgenbind-ast.c',
	'src/ir.c',
	'src/duk-libdom.c',
	'src/duk-libdom-interface.c',
	'src/duk-libdom-dictionary.c',
	'src/duk-libdom-common.c',
	'src/duk-libdom-generated.c'
)

nsgenbind = executable('nsgenbind',
    nsgenbind_src,
    nsgenbind_lexer,
    nsgenbind_parser,
    webidl_lexer,
    webidl_parser,
    include_directories: include_directories('src')
)