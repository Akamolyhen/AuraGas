--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--
local Util = require("Util")
---@type WBP_AttributesMenu_C
local M = UnLua.Class()


function M:WaitSeconds(duration)
    UE.UKismetSystemLibrary.Delay(self,duration)
end

-- 使用示例
function M:StartAsyncTask()
    local co = coroutine.create(function()
        local input, userInput =  UE.UUIFrameLibrary.CreateWaitMessageBox(self,"Hello Wait", UE.FLatentActionInfo(), UE.EUserInput.Type_None,1)
        if userInput == UE.EUserInput.OnUserResponse then
            if input == 0 then
                print("取消")
            end
            if input == 1 then
                print("确认")
            end
        end
        --Util:ScreenPrint("Hello World")
        --self:WaitSeconds(3)
        --Util:ScreenPrint("Hello World 3")
    end
    )
    coroutine.resume(co)
end


function M:Initialize(Initializer)
end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    self.Btn_CloseAttribute.Button.OnClicked:Add(self,self.CloseMenu)
    self:SetWidgetController(UE.UAuraAbilitySystemLibrary.GetAttributeMenuWidgetController(self))
    local playerController = UE.UGameplayStatics.GetPlayerController(self,0)
    ---@type BP_AuraHUD_C
    local AuraHud = playerController:GetHUD()
    AuraHud:SetMenuOpen(true)
    --UE.UUIFrameLibrary.CreateMessageBox(self,function() end, function() end,"是否关闭属性菜单")
end

function M:CloseMenu()
    self:RemoveFromParent()
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:Destruct()
    self.OnAttributeMenuClosed:Broadcast()
    local playerController = UE.UGameplayStatics.GetPlayerController(self,0)
    if not playerController then
        return
    end
    ---@type BP_AuraHUD_C
    local AuraHud = playerController:GetHUD()
    AuraHud:SetMenuOpen(false)
end

--对Attribute数据进行初始化
function M:WidgetControllerSet()
    self:BindInitAttributes()
    ---@type BP_AttributeMenuWidgetController_C
    local widgetController = self.WidgetController
    widgetController:BroadcastInitialValues()
end


return M

