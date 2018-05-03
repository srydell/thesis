import pathlib
import pickle
import configparser
import inspect

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
    """Load obj from the filesystem (relative to the script location)

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

def _createConfigs(configFile):
    """Create config file, used to programmatically create the config file.
    OBS: Writes all values to default.

    :configFile: String - Filename and/or path to config file
    :returns: None
    """
    config = configparser.ConfigParser()

    config['isingWorm.py'] = {"Plotting" : True, "Save data" : False, "Debug" : False, "Boundary condition": "dirichlet"}

    config['indexClusters.py'] = {"Debug" : False}

    config['graphs.py'] = {"Debug" : False}

    config['plotGraph.py'] = {"Debug" : False}

    config['utils.py'] = {"Debug" : False}

    with open(configFile, 'w') as c:
        config.write(c)

def loadConfigs(configFile):
    """Load the config from configFile into a dictionary.
    Only loads the config associated with the callers function name.

    :configFile: String - Filename and/or path to config file
    :returns: dictionary - Nicely formatted configs
    """
    config = configparser.ConfigParser()
    config.read(configFile)

    # Get the callers filename
    # This is done so that files can only access their part of the configs
    frame = inspect.stack()[1]
    module = inspect.getmodule(frame[0])
    callerFilename = module.__file__

    if not callerFilename in config.sections():
        raise Exception(f"{callerFilename} not in available configs: {config.sections()}")

    outDict = {}
    for section in config[callerFilename]:
        # Format the dictionary more pythonic
        if config[callerFilename][section] in ["True", "true", "on"]:
            outDict[section] = True
        elif config[callerFilename][section] in ["False", "false", "off"]:
            outDict[section] = False
        else:
            outDict[section] = config[callerFilename][section]

    return outDict

if __name__ == '__main__':

    answer = input("Do you want to write defaults to config? [y/n] ")
    if answer in ['y', 'Y']:
        configFile = "config.ini"
        print(f"Overwriting config file {configFile}")

        _createConfigs(configFile)
