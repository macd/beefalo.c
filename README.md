# Beefalo, an old Bison modified to produce Julia hosted parsers

This is a modified version of an old bison (bison-1.35 circa 2002)
that has been modified to generate parsers with Julia as the
implementation language, ie it is a bison derivative (and hence
"beefalo").

**This version NO LONGER SUPPORTS Parsers implemented with C or C++.**
If you want that, use an unmodified and more modern
version of bison. Truth in advertising: these generated parsers are
not always performant, but they can be easy to put together.

The reason that such an old bison was used is that 1.35 was the last
version to have a separate "skeleton" parsing file, ie "bison.simple"
and "bison.hairy". After that release, bison started using a heavily
macro-tized skeleton file which was run multiple times through the m4
macro processor and then included directly into the executable.  This
made it fairly impenetrable (at least to me).

In order to support a new host language for the generated compiler,
the skeleton file needs to be written in that language. Here we have a
new skeleton file, written in Julia, called beefalo.simple. Note that
this is included directly into the executable, so no need to copy the
skeleton file to a known location or specify it on the command line if
you are using the default.

## Using Beefalo

The bison 'language' has been left the same.  That is, the grammer and
the code parts still use C style comments.  Look in the Bison docs,
which can be found in the docs directory for Bison 1.35, for the
grammer specification.

However, all the code you write in the actions will be Julia code.
But there are a few issues to work around.

The Bison language is a very simple language.  Unfortunately for
Julia, both '$' and '@' are reserved characters in Bison with special
meaning, so don't use interpolation or macros in the grammar body.
Also note that Bison does not interpret these correctly inside of
quotes "".  So you will need to move much of your semantic actions
outside of the grammer specification, which is good practice anyway.

The semantic value stack has been set to String so that all interpretation
of a token's value should happen in the parser, not the lexer. I tried
to use some of Bison's support for union types in the semantic values,
but it proved to be a headache.

To turn on the printing of a parsing trace set yydebug = true in
the initial code section of the .y file.  This is macro defined away
so there is no performance impact if you choose not to use it.

Location tracking has been turn off by default. There is some support
left in, but you'll have to read the template file to learn how to
use it.

## Dependencies
Since the generation of the parsing tables in the C code uses all zero
based indexing, we depend on OffsetArrays to make zero based arrays
for these tables.

## Lexing

The parsing routine **yyparse** now expects to be passed the name of
the lexing function, which should take no arguments.  There is an
additional optional argument for yyparse that can be anything you want
and is there mainly for communication between the parser and the lexer
(for [context-sensitive lexing](https://en.wikipedia.org/wiki/Lexical_analysis#Context-sensitive_lexing)).
The lexer needs to return a tuple (tok, tok_val, loc) of token and the
token semantic value, which should be a string. If the location
tracking is turned on then a location struct must be returned in loc,
otherwise return a 'nothing'.

By using myuopt in both the parser and the lexer, you can change the
behavior of the lexer (context-sensitive lexing mentioned earlier).

The best bet for generating a lexer in Julia at the present time seems to
be the package Automa.jl (really a nice piece of work).

## Supported platforms
Well, you might think that resurrecting a 20 year old autotools based 
build might not go well and you would be right. So the only supported 
platforms are linux on x86_64 (with glibc) and linux on aarch64 (also 
with glibc)

## This is a Hack
In case it's not yet apparent, this is a hack, although someone might
find it useful.  So, none of the testing has been ported over to the
Julia version (yet?).

