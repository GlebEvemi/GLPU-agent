# =========================
# COMPUTER INFO COLLECTION
# =========================

# Basic computer info
$computerInfo = Get-ComputerInfo

# Processor info
$cpu = Get-CimInstance Win32_Processor | Select-Object -First 1

# Disk info (берем системный диск)
$disk = Get-CimInstance Win32_LogicalDisk -Filter "DeviceID='C:'"

# RAM conversion
$totalRamGb = [math]::Round($computerInfo.CsTotalPhysicalMemory / 1GB, 2)

# Disk conversion
$diskTotalGb = [math]::Round($disk.Size / 1GB, 2)
$diskFreeGb = [math]::Round($disk.FreeSpace / 1GB, 2)

# Create object
$result = @{
    Hostname          = $computerInfo.CsName
    OSName            = $computerInfo.OsName
    OSVersion         = $computerInfo.OsVersion
    BiosManufacturer  = $computerInfo.BiosManufacturer
    BiosStatus        = $computerInfo.BiosStatus
    CsDomain          = $computerInfo.CsDomain
    CsModel           = $computerInfo.CsModel
    csProcessorName   = $cpu.Name
    totalRam_Gb       = $totalRamGb
    diskTotalSize     = $diskTotalGb
    diskFreeGb        = $diskFreeGb
}

# Convert to JSON
$result | ConvertTo-Json -Compress