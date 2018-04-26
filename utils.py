import pathlib
import pickle

def saveObject(obj, filename="obj"):
    """Save obj to the filesystem (relative to the script location)
    NOTE: If given a filepath that doesn't exist, it will create it.

    :obj: Some datastructure - NOTE: Must be able to pickle
    :filename: String - Optional (default: obj)
    :returns: None
    """

    path = pathlib.Path(filename)
    # Create parent directories. If they exist, do nothing
    path.parent.mkdir(parents=True, exist_ok=True)
    with open(f"./{filename}.pickle", "wb") as f:
        pickle.dump(obj, f, protocol=pickle.HIGHEST_PROTOCOL)

def loadObject(filename="obj"):
    """Load obj from the filesystem

    :filename: String - Optional (default: obj)
    :returns: dictionary - The loaded obj
    """
    path = pathlib.Path(filename)

    if path.exists():
        with open(f"./{filename}.pickle", "rb") as f:
            obj = pickle.load(f)
        return obj
    else:
        raise Exception(f"Loaded file does not exist. Tried to load {filename}.pickle")

