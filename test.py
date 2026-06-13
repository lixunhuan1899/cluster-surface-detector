from cuni_ads_scripts.surface_atoms import get_surface_mask
from ase.io import write, read

atoms = read('test.xyz', index=-1)
positions = atoms.positions
is_surface = get_surface_mask(positions)
atoms.arrays['is_surface'] = is_surface

write("surface_test.xyz", atoms)