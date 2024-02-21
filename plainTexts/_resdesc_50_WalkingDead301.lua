local set = {}
set.name = "WalkingDead301"
set.setName = "WalkingDead301"
set.descriptionFilenameOverride = ""
set.logicalName = "<WalkingDead301>"
set.logicalDestination = "<>"
set.priority = 101
set.localDir = _currentDirectory
set.enableMode = "bootable"
set.version = "trunk"
set.descriptionPriority = 0
set.gameDataName = "WalkingDead301 Game Data"
set.gameDataPriority = 0
set.gameDataEnableMode = "constant"
set.localDirIncludeBase = true
set.localDirRecurse = false
set.localDirIncludeOnly = nil
set.localDirExclude = 
{
    "Packaging/",
    "_dev/"
}
set.gameDataArchives = 
{
    _currentDirectory .. "WDC_pc_WalkingDead301_anichore.ttarch2",
    _currentDirectory .. "WDC_pc_WalkingDead301_data.ttarch2",
    _currentDirectory .. "WDC_pc_WalkingDead301_ms.ttarch2",
    _currentDirectory .. "WDC_pc_WalkingDead301_txmesh.ttarch2",
    _currentDirectory .. "WDC_pc_WalkingDead301_voice.ttarch2"
}
RegisterSetDescription(set)
