import time

data = (time.strftime("#define DATA_COMPILA \"%d/%m/%Y\""))
ora  = (time.strftime("#define ORA_COMPILA  \"%H:%M:%S\""))
seq = [data, '\n', ora]
text_file = open("src\data_ora_compila.h", "w")
text_file.writelines(seq)
text_file.close()