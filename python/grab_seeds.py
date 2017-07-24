import glob, os

def grab_seeds(folder='.'):
  """Returns the set of seeds for wagner simulations."""
  seeds = set()
  os.chdir(folder)
  for file in glob.glob('*.xml'):
    if file[2] != 's':
      seeds.add(int(file[2:-4]))
  return seeds


