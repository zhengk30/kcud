def read_lines(path):
    result = []
    with open(path, 'r') as file:
        for row in file:
            entries = row.split(', ')
            for i in range(len(entries)):
                entries[i] = entries[i].rstrip('\n')
            entries = tuple(entries)
            result.append(entries)
        file.close()
    return result


def read_nums(path):
    result = []
    with open(path, 'r') as file:
        for row in file:
            result.append(row.rstrip('\n'))
        file.close()
    return result


def compare_rows(file1, file2):
    result1 = read_lines(file1)
    result2 = read_lines(file2)
    success = 1
    for line in result1:
        if line not in result2:
            print(line)
            success = 0
    return True if success == 1 else False


def compare_nums(file1, file2):
    result1 = read_nums(file1)
    result2 = read_nums(file2)
    success = 1
    for i in range(len(result1)):
        if result1[i] != result2[i]:
            print(result1[i])
            success = 0
    return True if success == 1 else False


if __name__ == '__main__':
    file1 = 'actual-offsets.txt'
    file2 = 'expected-offsets.txt'
    print(compare_nums(file1, file2))
