--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type VideoUI_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    self.file = 1
    self.Btn_OpenFile.OnClicked:Add(self,self.OpenFile)
    self.Btn_ChangePath.OnClicked:Add(self,self.ChangePath)
end

function M:ChangePath()
    local paths = {
        "C:/Users/aka_w/Downloads/aigei_com.mp4",
        "C:/Users/aka_w/Downloads/aigei_com1.mp4"
    }
    self.FilePath:SetText(paths[self.file])
    self.file = (self.file + 1) % 2 == 0 and 2 or 1
end

function M:OpenFile()
    print("当前打开路径"..self.FilePath:GetText())
    if self.mediaPlayer:OpenFile(self.FilePath:GetText()) then
        print("play")
    else
        print("not play")     
    end
end
--function M:Tick(MyGeometry, InDeltaTime)
--end

return M
