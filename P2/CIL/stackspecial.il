.assembly extern mscorlib
{
}
.assembly App
{
}



// =============== CLASS MEMBERS DECLARATION ===================

.class private auto ansi beforefieldinit ConsoleApplication2.Program
       extends [mscorlib]System.Object
{
  .method private hidebysig static void  Main(string[] args)
  {
    .entrypoint
    .maxstack 27 //uma para cada variavel? (repensar)

    .locals init ([0] class Auxiliar.SpecialStack<int32> a)

    // cria a pilha e guarda na variavel a
    newobj instance void class Auxiliar.SpecialStack<int32>::.ctor()
    stloc.0

    // load variavel a 
    ldloc a

    // load constante 123
    ldc.i4.s 123
    box [mscorlib]System.Int32

    // push (instance = a e object = 123)
    callvirt instance void [mscorlib]System.Collections.Stack::Push(object)

    // load variavel a 
    ldloc a

    // load constante 456
    ldc.i4 456
    box [mscorlib]System.Int32

    // push (instance = a e object = 456)
    callvirt instance void [mscorlib]System.Collections.Stack::Push(object)

    // agora a contém [123 456_]
    ldloc.0
   
    // pop em a (e joga valor retornado na stack)
    callvirt instance object [mscorlib]System.Collections.Stack::Pop()

    // imprime topo da pilha
    call void [mscorlib]System.Console::WriteLine(object)

    // load variavel a 
    ldloc a

    // pop em a (e joga valor retornado na stack)
    callvirt instance object [mscorlib]System.Collections.Stack::Pop()

    // imprime topo da pilha
    call void [mscorlib]System.Console::WriteLine(object)



    //-------------------pop pilha vazia (testando funcionalidades do specialstack)
    // load variavel a 
    ldloc a
    // pop em a (e joga valor retornado na stack)
    callvirt instance object [mscorlib]System.Collections.Stack::Pop()
    // imprime topo da pilha
    call void [mscorlib]System.Console::WriteLine(object)

    // dando peek em pilha vazia
     ldloc a
    // pop em a (e joga valor retornado na stack)
    callvirt instance object [mscorlib]System.Collections.Stack::Peek()
    // imprime topo da pilha
    call void [mscorlib]System.Console::WriteLine(object)


  } // end of method Program::Main

} // end of class ConsoleApplication2.Program






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

