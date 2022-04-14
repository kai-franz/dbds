opt -dot-cfg $1 -cfg-dot-filename-prefix $1
dot -Tpng -o $1.png $1.*.dot
