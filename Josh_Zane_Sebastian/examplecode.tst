^ = (a b) {
    < $! $| $! a $! b
}

^2 = (a b) {
    na = ! a
    nb = ! b
    naonb = | na nb
    < $! naonb
}

xor = (a b) {
    < $| $^ b $! a $^ a $! b
}
