.assembly extern mscorlib
{
}
.assembly App
{
}


// ========================= AUXILIAR CLASS ====================================
// special stack returns 0 when Pop() or Peek() is called for an empty stack

.class public auto ansi beforefieldinit Auxiliar.SpecialStack<T>
       extends [mscorlib]System.Collections.Stack
{
  .method public hidebysig virtual instance object 
          Pop() cil managed
  {
    // Code size       25 (0x19)
    .maxstack  1
    .locals init ([0] class [mscorlib]System.InvalidOperationException e,
             [1] object CS$1$0000)
    IL_0000:  nop
    .try
    {
      IL_0001:  nop
      IL_0002:  ldarg.0
      IL_0003:  call       instance object [mscorlib]System.Collections.Stack::Pop()
      IL_0008:  stloc.1
      IL_0009:  leave.s    IL_0016

    }  // end .try
    catch [mscorlib]System.InvalidOperationException 
    {
      IL_000b:  stloc.0
      IL_000c:  nop
      IL_000d:  ldc.i4.0
      IL_000e:  box        [mscorlib]System.Int32
      IL_0013:  stloc.1
      IL_0014:  leave.s    IL_0016

    }  // end handler
    IL_0016:  nop
    IL_0017:  ldloc.1
    IL_0018:  ret
  } // end of method SpecialStack::Pop

  .method public hidebysig virtual instance object 
          Peek() cil managed
  {
    // Code size       25 (0x19)
    .maxstack  1
    .locals init ([0] class [mscorlib]System.InvalidOperationException e,
             [1] object CS$1$0000)
    IL_0000:  nop
    .try
    {
      IL_0001:  nop
      IL_0002:  ldarg.0
      IL_0003:  call       instance object [mscorlib]System.Collections.Stack::Peek()
      IL_0008:  stloc.1
      IL_0009:  leave.s    IL_0016

    }  // end .try
    catch [mscorlib]System.InvalidOperationException 
    {
      IL_000b:  stloc.0
      IL_000c:  nop
      IL_000d:  ldc.i4.0
      IL_000e:  box        [mscorlib]System.Int32
      IL_0013:  stloc.1
      IL_0014:  leave.s    IL_0016

    }  // end handler
    IL_0016:  nop
    IL_0017:  ldloc.1
    IL_0018:  ret
  } // end of method SpecialStack::Peek

  .method public hidebysig specialname rtspecialname 
          instance void  .ctor() cil managed
  {
    // Code size       7 (0x7)
    .maxstack  8
    IL_0000:  ldarg.0
    IL_0001:  call       instance void [mscorlib]System.Collections.Stack::.ctor()
    IL_0006:  ret
  } // end of method SpecialStack::.ctor

} // end of class Auxiliar.SpecialStack


// =============================================================


// =============== CLASS MEMBERS DECLARATION ===================

.class private auto ansi beforefieldinit ConsoleApplication2.Program
       extends [mscorlib]System.Object
{
  .method private hidebysig static void  Main(string[] args)
  {
      .entrypoint
      .maxstack 10

      .locals init ([0] class Auxiliar.SpecialStack<int32> a,
                    [1] class Auxiliar.SpecialStack<int32> b,
                    [2] class Auxiliar.SpecialStack<int32> c,
                    [3] class Auxiliar.SpecialStack<int32> d,
                    [4] class Auxiliar.SpecialStack<int32> e,
                    [5] class Auxiliar.SpecialStack<int32> f,
                    [6] class Auxiliar.SpecialStack<int32> g,
                    [7] class Auxiliar.SpecialStack<int32> h,
                    [8] class Auxiliar.SpecialStack<int32> i,
                    [9] class Auxiliar.SpecialStack<int32> j,
                    [10] class Auxiliar.SpecialStack<int32> k,
                    [11] class Auxiliar.SpecialStack<int32> l,
                    [12] class Auxiliar.SpecialStack<int32> m,
                    [13] class Auxiliar.SpecialStack<int32> n,
                    [14] class Auxiliar.SpecialStack<int32> o,
                    [15] class Auxiliar.SpecialStack<int32> p,
                    [16] class Auxiliar.SpecialStack<int32> q,
                    [17] class Auxiliar.SpecialStack<int32> r,
                    [18] class Auxiliar.SpecialStack<int32> s,
                    [19] class Auxiliar.SpecialStack<int32> t,
                    [20] class Auxiliar.SpecialStack<int32> u,
                    [21] class Auxiliar.SpecialStack<int32> v,
                    [22] class Auxiliar.SpecialStack<int32> w,
                    [23] class Auxiliar.SpecialStack<int32> x,
                    [24] class Auxiliar.SpecialStack<int32> y,
                    [25] class Auxiliar.SpecialStack<int32> z,
                    [26] class Auxiliar.SpecialStack<int32> @
      )
    
      // cria as pilhas
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor() 
      stloc a
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()  
      stloc b
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc c
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor() 
      stloc d
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc e    
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc f
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc g
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc h
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc i
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc j
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc k
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc l
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc m
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc n
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc o
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc p
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc q
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc r
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc s
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc t
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc u
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc v
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc w
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc x
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc y
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc z
      newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
      stloc @

      // -- aqui começa a área de código


  } // end of method Program::Main

} // end of class ConsoleApplication2.Program

