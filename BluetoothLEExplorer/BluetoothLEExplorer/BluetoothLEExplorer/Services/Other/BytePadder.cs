﻿// <copyright file="BytePadder.cs" company="Microsoft Corporation">
// Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//----------------------------------------------------------------------------------------------

namespace BluetoothLEExplorer.Services.Other
{
    /// <summary>
    /// Helper class used to pad bytes
    /// </summary>
    public static class BytePadder
    {
        /// <summary>
        /// Takes an input array of bytes and returns an array with more zeros in the front
        /// </summary>
        /// <param name="input"></param>
        /// <param name="length"></param>
        /// <returns>A byte array with more zeros in front"/></returns>
        public static byte[] GetBytes(byte[] input, int length)
        {
            byte[] ret = new byte[length];

            if (input.Length >= length)
            {
                return input;
            }
            
            for (int i = 0; i < input.Length; i++)
            {
                ret[i] = input[i];
            }

            return ret;
        }
    }
}
