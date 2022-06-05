from datetime import datetime
now = datetime.now()  # current date and time
time = now.strftime("%H%M")
dateP = str(now.year + 543) + now.strftime("%m%d")
print("date and time:", dateP)
stringx = "{"\
    "    \"data\":{"\
    "         \"Serial_No\":\"0020130260\","\
    "         \"IMEI\":\"865057043207871\","\
    "         \"CurrReadDate\":\""+dateP+"\","\
    "         \"Seq\":\"1\","\
    "         \"CurrReadNum\":\"iamgique\","\
    "         \"CurrReadTime\":\""+time+"\","\
    "         \"PctOfBatteryMeter\":\"\","\
    "         \"PctOfBatteryLogger\":\"096\","\
    "         \"PctOfSignal\":\"085\","\
    "         \"WaterPressure\":\"\","\
    "         \"FlowRate\":\"\","\
    "         \"Turbidity\":\"\","\
    "         \"Salinity\":\"\","\
    "         \"Chlorine\":\"\","\
    "         \"SendFromServer\":\"\","\
    "         \"Provider_Name\":\"Universal Utilities\""\
    "    }"\
    "}"
print(stringx)
