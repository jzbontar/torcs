require('cunn')
require('cudnn')
require('image')

local base_dir = '/home/jure/devel/torcs'
local net = torch.load(base_dir .. '/net/net.t7')
local x_batch = torch.CudaTensor(1, 3, 120, 160)
local img_mean = torch.load(base_dir .. '/data/img_mean.t7'):view(1, 3, 1, 1):expandAs(x_batch):cuda()

function drive(img)
   img = image.vflip(img:view(120, 160, 3):permute(3, 1, 2))
   x_batch:copy(img):add(-1, img_mean)
   net:forward(x_batch)
   angle = net.output[1]
   return angle
end
