# Собираем базовую инфу
$info = Get-ComputerInfo

$ramBytes = $info.CsTotalPhysicalMemory
$ramGB = [math]::Round($ramBytes / 1GB, 2)


$disks = Get-CimInstance Win32_LogicalDisk -Filter "DriveType=3" | ForEach-Object {
    [PSCustomObject]@{
        Drive       = $_.DeviceID
        SizeGB      = [math]::Round($_.Size / 1GB, 2)
        FreeGB      = [math]::Round($_.FreeSpace / 1GB, 2)
        UsedGB      = [math]::Round(($_.Size - $_.FreeSpace) / 1GB, 2)
    }
}

# Формируем объект (оставь только нужные поля)
$result = [PSCustomObject]@{
    Hostname            = $env:COMPUTERNAME
    OSName              = $info.OsName
    OSVersion           = $info.OsVersion
    BiosManufacturer    = $info.BiosManufacturer
    BiosFirmwareType    = $info.BiosFirmwareType
    BiosVersion         = $info.BiosBIOSVersion
    BiosStatus          = $info.BiosStatus
    CsBootupState       = $info.CsBootupState
    CsDNSHostname       = $info.CsDNSHostname
    CsDomain            = $info.CsDomain
    CsDomainRole        = $info.CsDomainRole
    CsManufacturer      = $info.CsManufacturer
    CsModel             = $info.CsModel
    CsName              = $info.CsName
    CsNumberOfLogicalProcessors = $info.CsNumberOfLogicalProcessors
    CsNumberOfProcessors = $info.CsNumberOfProcessors
    CsProcessors        = $info.CsProcessors
    CsStatus            = $info.CsStatus
    TotalRAM_GB         = $ramGB
    
    Disks               = $disks
}

$result | ConvertTo-Json -Depth 3
