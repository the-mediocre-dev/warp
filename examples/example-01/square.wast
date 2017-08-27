(module
  (type (;0;) (func (param i32) (result i32)))
  (func $square (type 0) (param i32) (result i32)
    get_local 0
    get_local 0
    i32.mul)
  (table (;0;) 0 anyfunc)
  (memory (;0;) 0)
  (export "square" (func $square)))
