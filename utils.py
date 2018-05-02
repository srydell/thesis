import pathlib
import pickle

def addIfNotExists(key, value, dictionary):
    """Adds the entry (key: value) to dictionary if not exists
    If it dictionary[key] exists and is a list, append value to it

    :key: Something hashable
    :value: Some value to be stored
    :dictionary: dictionary
    :returns: None
    """

    if dictionary.get(key) is None:
        dictionary[key] = value
        return

    elif type(dictionary[key]) == int or type(dictionary[key]) == float:
        dictionary[key] += value
        return

    elif type(dictionary[key]) == list:
        # Only append if it is not already there
        if value not in dictionary[key]:
            dictionary[key].append(value)

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

