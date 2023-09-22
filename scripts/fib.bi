one = 0
two = 1
next = two

6 : [] {
  result = one
  next = one + two
  one = two
  two = next
  result
}