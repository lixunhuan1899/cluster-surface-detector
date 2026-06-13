import ctypes
import numpy as np
from pathlib import Path

lib = ctypes.CDLL(str(Path(__file__).parent / 'libsurface.so'))

_surface_atoms = lib.surface_atoms

_surface_atoms.argtypes = [
    np.ctypeslib.ndpointer(dtype=np.float64, flags='C_CONTIGUOUS'),
    ctypes.c_int,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_int,
    np.ctypeslib.ndpointer(dtype=np.bool_, flags='C_CONTIGUOUS')
]

_surface_atoms.restype = None


def get_surface_mask(positions, min_r=4.0, max_r=4.5, n_tries=1000000):
    positions = np.ascontiguousarray(positions, dtype=np.float64)

    mask = np.zeros(len(positions), dtype=np.bool_)

    _surface_atoms(positions.ravel(), len(positions), min_r, max_r, n_tries, mask)

    return mask


def get_surface_atoms(atoms, min_r=4.0, max_r=4.5, n_tries=1000000):
    mask = get_surface_mask(atoms.positions, min_r, max_r, n_tries)

    return np.where(mask)[0]
