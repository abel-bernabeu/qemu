/*
 * RISC-V translation routines for the DGX extension.
 *
 * Copyright (c) 2022 Abel Bernabeu
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
1.2. Extension Listing Instruction
Query the unique extension identifier contained at given entry in the array of supported extensions.

xlist rd, rs # Queries the rs-th supported extension

Looks up the rs-th element on the array of supported extensions and stores its unique extension identifier value into rd. Because the extension identifiers are unsigned 32-bits integers, they need to be zero-extended in case XLEN is 64 bits. If the rs index is beyond the last element in the array of supported extensions then the xlist instruction returns zero.

When the software needs to find out the size of the array of supported extensions it does so by executing xlist in a loop until it returns zero.
*/

static bool gen_xlist(DisasContext *ctx, arg_r *a)
{
    static const uint32_t dgxExtensions[] = {1, 2, 3};
    const size_t size = sizeof(dgxExtensions) / sizeof(dgxExtensions[0]);

    TCGv ret = get_gpr(ctx, a->rd, EXT_ZERO);

    for (size_t index = 0; index < size; ++index) {
        TCGCond cond = TCG_COND_EQ;
        TCGv c1 = get_gpr(ctx, a->rs1, EXT_ZERO);
        TCGv c2 = tcg_constant_tl(index);
        TCGv v1 = tcg_constant_tl(dgxExtensions[index]);
        TCGv v2;
        if (index == 0) {
            v2 = tcg_constant_tl(0);
        } else {
            v2 = ret;
        }
        tcg_gen_movcond_tl(cond, ret, c1, c2, v1, v2);
    }

    gen_set_gpr(ctx, a->rd, ret);
    return true;
}

static bool trans_xlist(DisasContext *ctx, arg_r *a)
{
    return gen_xlist(ctx, a);
}