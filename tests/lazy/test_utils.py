# -*- coding: utf-8 -*-
"""Test Op utils.

"""
from starry.ops import block_diag


def test_block_diag():
    C1 = np.ones((2, 2))
    C2 = np.ones((3, 3)) * 2
    C3 = np.ones((3, 3)) * 3
    C = scipy_block_diag(C1, C2, C3)

    C1 = tt.as_tensor_variable(np.ones((2, 2)))
    C2 = tt.as_tensor_variable(np.ones((3, 3)) * 2)
    C3 = tt.as_tensor_variable(np.ones((3, 3)) * 3)

    assert np.allclose(C, block_diag(C1, C2, C3).eval())
