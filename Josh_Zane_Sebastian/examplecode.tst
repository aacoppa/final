^ = fn(a, b) {
    return ~(|(~(a), ~(b)));
}

XOR = fn(c, d) {
    return fn(^, a, b) {
    	  return |(^(a, b), ^(~(a), ~(b)));
    } (fn(a, b) { return ~(|(~(a), ~(b)));}, c, d);
}

XOR = fn(a, b, ^) {
    return |(^(a, b), ~(|(a, b)));
}
