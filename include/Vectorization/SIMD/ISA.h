/**
 * File              : ISA.h
 * Author            : Marcos Horro <marcos.horro@udc.gal>
 * Date              : Mar 31 Dec 2019 16:50:47 MST
 * Last Modified Date: Mar 31 Dec 2019 16:57:51 MST
 * Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
 */

namespace macveth {

/// ISAs available for this compiler
enum ISA { SSE, AVX, AVX2 };

/// Architectures known by the compiler
enum Arch { X86 };

/// TODO get architecture
Arch getArchitecture() { return Arch::X86; }

} // namespace macveth
