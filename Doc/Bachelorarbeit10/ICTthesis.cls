\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{ICTthesis}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{report}}
\ProcessOptions
\LoadClass{report}

% Chapters:
\renewcommand{\@makechapterhead}[1]{%
  \vspace*{84\p@}%
  {\parindent \z@ \raggedright \normalfont
    \interlinepenalty\@M
    \huge\bfseries \scshape \thechapter \hspace{0.2 cm} #1\par
    \vskip 48\p@
  }}
\renewcommand{\@makeschapterhead}[1]{%
  \vspace*{84\p@}%
  {\parindent \z@ \raggedright \normalfont
    \interlinepenalty\@M
    \huge\bfseries \scshape  #1\par
    \vskip 48\p@
  }}

\endinput
