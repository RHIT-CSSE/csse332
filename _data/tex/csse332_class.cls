%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CSSE332 Worksheet Class
% LaTeX Class
% Version 0.06 (January 06, 2024)
%
% Author:
% Mohammad A. Noureddine (noureddi@rose-hulman.edu)
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%------------------------------------------------------------------------------
%   CLASS CONFIGURATION
%------------------------------------------------------------------------------

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{csse332_class}[%
  2024-01-06 v0.05 Lab class for operating systems worksheet.%
]

% pass options to the exam package
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{exam}}

% process all the options, this executes the code for each option that was both
% specified and declared.
\ProcessOptions\relax

% Load the exam package with 11pts and points options.
\LoadClass[11pt,addpoints]{exam}

%------------------------------------------------------------------------------
%   REQUIRED PACKAGES
%------------------------------------------------------------------------------

\RequirePackage{xspace}           % need this for the macros we define.
\RequirePackage{amsmath,amssymb}  % general math symbols.
\RequirePackage{graphicx}         % graphics for adding figures.
\RequirePackage{textcomp}         % needed for upquote in listings.
\RequirePackage{listings}         % use listings for code snippets.
\RequirePackage[x11names]{xcolor} % use colors for lstlistings config.

% set the margins, I normally like 1in everywhere.
\usepackage[%
  top=1in,
  bottom=1in,
  left=1in,
  right=1in,
]{geometry}

% check if fancybox is available and load it, otherwise skip
\IfFileExists{fancybox.sty}{%
  \usepackage{fancybox}
}{%
  \ClassWarning{netsec_lab}{Fancybox was not detected, skipping over it.}
}


%------------------------------------------------------------------------------
%   DEFINE COMMANDS
%------------------------------------------------------------------------------

\providecommand{\term}{No term defined}              % course term
\providecommand{\course}{\textsc{Operating Systems}} % course name
\providecommand{\labdate}{\today}                    % lab date
\providecommand{\labnumber}{Worksheet ??}            % lab number

\providecommand{\ci}[1]{{\small \texttt{#1}}} % code in text
\providecommand{\pp}[1]{\noindent {\bf #1}}   % section-like paragraph
\providecommand{\eg}{e.g., \xspace}           % e.g., in text
\providecommand{\ie}{i.e., \xspace}           % i.e., in tex

%------------------------------------------------------------------------------
%   PAGE CONFIGURATIONS
%------------------------------------------------------------------------------

% set default page style to header and footer to include term and class configs.
\pagestyle{headandfoot}

% set header and footer
\header{\term}{Name: \rule{2.5in}{0.5pt}}{\course}
\coverheader{\term}{CSSE 332}{\course}
\footer{\labdate}{Day \labnumber}{Page \thepage\xspace of \numpages}

\providecommand{\namebox}{%
  \vspace{.5in}
  \begin{center}
    \large
    Name:\enspace\makebox[3.7in]{\hrulefill}\hspace{0.25in}%
  \end{center}
  \vspace{.5in}
}

% config exam package question to show as problem with arabic numbering
\renewcommand{\thequestion}{\textbf{Question\xspace \arabic{question}}}

%------------------------------------------------------------------------------
%   TITLE PAGE CONFIGURATION
%------------------------------------------------------------------------------
\renewcommand{\maketitle}{%
  \begin{center}
    \huge
    \vspace*{1em}
    \texttt{CSSE 332 -- \course}\\
    \vspace{.9em}
    \texttt{Rose-Hulman Institute of Technology}\\
  \end{center}
  \vspace*{-1.0\parskip}
  \begin{center}
    \huge \@title
  \end{center}%
  \vspace*{1.0\parskip}
}

%------------------------------------------------------------------------------
%   LSTLISTINGS SETTINGS
%------------------------------------------------------------------------------
\definecolor{codegreen}{rgb}{0,0.6,0}
\definecolor{codegray}{rgb}{0.5,0.5,0.5}
\definecolor{codepurple}{rgb}{0.58,0,0.82}
\definecolor{backcolour}{rgb}{0.95,0.95,0.92}

% define our own style for the netsec labs
\lstdefinestyle{netseclab}{%
  backgroundcolor=\color{backcolour},
  basicstyle=\tt\footnotesize,
  tabsize=2,
  numbers=left,
  stringstyle=\color{codepurple},
  keywordstyle=\color{blue},
  numberstyle=\footnotesize\color{codegray},
  commentstyle=\color{codegreen},
  numbersep=4pt,
  breaklines=true,
  keepspaces=true,
  showstringspaces=false,
  upquote=true,
  frame=single,
}

% define a language for showing shell commands
\lstdefinelanguage[]{shell}[]{bash}{%
  morekeywords={export,cd,pushd,popd,gcc,sudo,apt,bash,python},%
  sensitive=true,%
  morekeywords=[2]{$},%
  keywordstyle=[2]\bf\tt\color{DeepPink4},%
}[keywords,comments,strings]

% make code including easy
\newcommand{\includecode}[2][shell]{%
  \lstinputlisting[style=netseclab,language=#1]{#2}%
}

%------------------------------------------------------------------------------
%   COURSE AND LAB SPECIFIC DEFINITIONS
%------------------------------------------------------------------------------

% learning objectives block, I put it here since I am not fully convinced on the
% format and I need a way to change it globally.
\providecommand{\learningobjs}[1]{%
  \@ifpackageloaded{fancybox}{%
  \section*{Learning Objectives}
  \vspace{.3in}
  \begin{center}
  \doublebox{%
    \begin{minipage}{.8\textwidth}
      {\bf At the end of this lab, you should be able to:}
      \vspace{.8em}
      #1
      \vspace{.8em}
    \end{minipage}}
  \end{center}
  }{%
    \section*{Learning Objectives}
    \vspace{.3in}
    {\bf At the end of this lab, you should be able to:}
    \vspace{.8em}
    #1
    \vspace{.8em}
  }
}

%------------------------------------------------------------------------------
%   END
%------------------------------------------------------------------------------
\endinput

% CHANGELOG
%
% 19 Oct 2023, v0.01: Created file.
%
% 20 Oct 2023, v0.02: Added file configs, requires exam package and xspace.
%
% 26 Oct 2023, v0.03: Switched from quiz to lab to be consitent. Added title
%    page. Added lstlistings options to add code to the lab.
%
% 04 Nov 2023, v0.04: Added better learning objectives within a double box.
%    Adjusted the title page with more space and being its own separate thing.
%    Added name box and cover page.
%
% 30 Nov 2023, v0.05: Switched from problem to question for more consistency.
%
% 06 Jan 2024, v0.06: Moved from netsec to csse332.
%

