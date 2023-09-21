one = 0
two = 1
next = two

128 : [] {
  result = one
  next = one + two
  one = two
  two = next
  result
}