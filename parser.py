

class ParseError(Exception):
    pass

def csv_parse(line):
    result = None
    try:
        toks = line.split(',')
        result = [float(p) for p in toks] 
    except ValueError:
        print("Failed to parse line: {}".format(line))
    return result
