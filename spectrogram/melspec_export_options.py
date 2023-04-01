# classes for exporter configuration

class DataType:
    (FLOAT, FIXED32, FIXED16, FIXED8) = (0,1,2,3)
    
class ProjectType:
    (C, CPP ,ARDUINO) = (0,1,2)
    
class ProjectFiles:
    (LIBRARY, MAIN) = (1, 2)
    (ALL) = {LIBRARY, MAIN}
    
class ProjectOptions:
    project_type = ProjectType.C
    data_type = DataType.FLOAT 
    include_debug = False
    example_data = None
    convert_to_db = False
    example_type = float
    files = ProjectFiles.ALL
    baudrate = 9600