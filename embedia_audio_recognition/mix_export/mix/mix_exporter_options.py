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
    baudrate = 9600