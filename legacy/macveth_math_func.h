/**
 * File              : macveth_math_func.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Sáb 04 Xan 2020 21:03:02 MST
 * Last Modified Date: Sáb 04 Xan 2020 21:07:03 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

/// This header is intendede to be included in the file to compile and used
/// instead their predefined math functions for this case. This is because
/// MACVETH will look for these signatures and generate custom SIMD instructions
/// for them

/// Cosine
template <class T> T cos(T V);
/// Sinus
template <class T> T sin(T V);
/// Tangent
template <class T> T tan(T V);
/// Hyperbolic inverse tangent
template <class T> T atanh(T V);
