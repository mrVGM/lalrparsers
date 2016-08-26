# lalrparsers
c++ library for creating LALR Parsers

Usage:
Parsers are made using the function lalrParser::Parser lalrParser::create(string & def); which is declared in the file Parser.h
The string def is the definition of the grammar used to build the parser. It consist of rules separeted by newline character '\n'. A single consist of non-terminal symbol enclosed with <> followed by a space and sequence of symbols enclosed with double quotes. Characters inside the quotes can be escaped with \. Symbols consisting of more than one character are enclosed with <>, just as the non-terminal of the rule. Also every grammar definition should start with an initial rule of the type:

```
<Start> "<The real rule of the grammar><End>"
```

where the symbols <Start> and <End> should not occur in any of the other rules.

Here is an example of a grammar definition (for arithmetic expressions):

```
<S> "<Exp><END>"

<Exp> "<Exp>+<Term>"
<Exp> "<Exp>-<Term>"
<Exp> "<Term>"

<Term> "<Term>*<Factor>"
<Term> "<Term>/<Factor>"
<Term> "<Factor>"

<Factor> "x"
<Factor> "(<Exp>)"
<Factor> "-<Factor>"
```

Parsers can be serialized with the method string serialize(); in the lalrParser::Parser class and deserialized with the function lalrParser::Parser lalrParser::restore(string & ser); declared in Parser.h

Parsers can parse objects of class lalrParser::Symbols (or list of objects of type lalrParser::Symbol) declared again in Parser.h. The result of the parsing process is an object of class lalrParser::DerivationTree (declared in DerivationTree.h). There is a full example of parsing in main.cpp
