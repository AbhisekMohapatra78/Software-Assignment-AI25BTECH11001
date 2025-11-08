from PIL import Image
import numpy as np
import numpy.linalg as LA
import time 

start = time.time()


img = Image.open("../../figs/gauss.jpg").convert("L")

dat = np.asarray(img, dtype=float)
U, E, Vt = LA.svd(dat, full_matrices=False)

k = 1000
U_k = U[:, :k]
E_k = np.diag(E[:k])
Vt_k = Vt[:k, :]

dat_recon = U_k @ E_k @ Vt_k

dat_recon = np.clip(dat_recon, 0, 255).astype(np.uint8)

img_recon = Image.fromarray(dat_recon, mode='L')
#img_recon.show()

print(f"time taken {time.time()-start}");

