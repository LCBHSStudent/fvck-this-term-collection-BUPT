import sqlite3
import random

con = sqlite3.connect(r'C:\Users\RAZER\Desktop\py_draft\EmulatorConfig.db')
cur = con.cursor()
print("succeed to open database")

cityName = [
    "北京", "成都", "北海", "西安", "兰州", "重庆", 
    "长沙", "武汉", "天津", "乌鲁木齐", "西宁", "拉萨"
]

costTable = [
    [0, 1281, 1560, 748, 995, 1258, 947, 746, 106, 1793, 1128, 2902],
    [1281, 0, 976, 601, 741, 210, 826, 798, 1312, 2005, 875, 2266],
    [1560, 976, 0, 1315, 1595, 832, 700, 897, 1596, 2856, 1757, 2085],
    [748, 601, 1315, 0, 517, 555, 728, 546, 776, 1735, 657, 2420],
    [995, 741, 1595, 517, 0, 818, 1225, 1033, 1036, 1291, 203, 2013],
    [1258, 210, 832, 555, 818, 0, 644, 685, 1230, 2079, 964, 2735],
    [947, 826, 700, 728, 1225, 644, 0, 245, 937, 2406, 1307, 3111],
    [746, 798, 897, 546, 1033, 685, 245, 0, 749, 2232, 1441, 2924],
    [106, 1312, 1596, 776, 1036, 1230, 937, 749, 0, 1902, 1175, 2958],
    [1793, 2005, 2856, 1735, 1291, 2079, 2406, 2232, 1902, 0, 1390, 2548],
    [1128, 875, 1757, 657, 203, 964, 1307, 1441, 1175, 1390, 0, 1838],
    [2902, 2266, 2805, 2420, 2013, 2735, 3111, 2924, 2958, 2548, 1838, 0]
]

stat = '''INSERT INTO `Schedule` (
    TACNumber,
    FromStation,
    ToStation,
    StartH, StartM,
    ArriveH, ArriveM,
    TransportationType)
    VALUES(?,?,?,?,?,?,?,2)'''

i = 0
while i < 500:
    TACNumber = random.randint(1, 11451)
    FromCityId = random.randint(0, 11)
    DestCityId = random.randint(0, 11)
    while FromCityId == DestCityId:
        DestCityId = random.randint(0, 11)
    StartH = random.randint(0, 23)
    StartM = random.randint(0, 59)
    ArriveH = int((StartH * 60 + StartM + costTable[FromCityId][DestCityId]) / 60)
    ArriveM = (StartH * 60 + StartM + costTable[FromCityId][DestCityId]) % 60

    values = [
        TACNumber,
        cityName[FromCityId], cityName[DestCityId], 
        StartH, StartM, 
        ArriveH, ArriveM
    ]
    # print(values)
    cur.execute(stat, values)
    i += 1

con.commit()
con.close()