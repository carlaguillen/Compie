﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ConsoleApplication2
{
    class Program
    {
        static void Main(string[] args)
        {
            Auxiliar.SpecialStack<Int32> test = new Auxiliar.SpecialStack<Int32>();
            test.Push(2);
        }
    }
}

namespace Auxiliar
{
    public class SpecialStack<T> : System.Collections.Stack
    {
        public override Object Pop() { 
            try 
            {
                return base.Pop();
            } 
            catch (InvalidOperationException e)
            {
                return 0;
            }
        }
        public override Object Peek()
        {
            try
            {
                return base.Pop();
            }
            catch (InvalidOperationException e)
            {
                return 0;
            }
        }
    }
}
