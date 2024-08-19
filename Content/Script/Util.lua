local Util = {}
function Util:PT(t,str)
    print(str or "PrintTable")
    local print_t_cache={}
    local function sub_tablePrint(t,indent)
        if (print_t_cache[tostring(t)]) then
            print(indent.."*"..tostring(t))
        else
            print_t_cache[tostring(t)]=true
            if (type(t)=="table") then
                for pos,val in pairs(t) do
                    if (type(val)=="table") then
                        print(indent.."["..pos.."] => "..tostring(t).." {")
                        sub_tablePrint(val,indent..string.rep(" ",string.len(pos)+8))
                        print(indent..string.rep(" ",string.len(pos)+6).."}")
                    elseif (type(val)=="string") then
                        print(indent.."["..pos..'] => "'..val..'"')
                    else
                        print(indent.."["..pos.."] => "..tostring(val))
                    end
                end
            else
                print(indent..tostring(t))
            end
        end
    end
    if (type(t)=="table") then
        print(tostring(t).." {")
        sub_tablePrint(t,"  ")
        print("}")
    else
        sub_tablePrint(t,"  ")
    end
    print()
end

return Util