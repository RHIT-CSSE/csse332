#!/bin/bash


# pdf_dir="/Users/sajed/Git/Github/courses/csse332/csse332-private/Quiz/pdfs"
pdf_dir=$(find ${HOME} -name 'csse332-private' 2> /dev/null)
pdf_dir="${pdf_dir}/Quiz/pdfs"

echo -e ${pdf_dir}

rsync -avP "$pdf_dir/" .
