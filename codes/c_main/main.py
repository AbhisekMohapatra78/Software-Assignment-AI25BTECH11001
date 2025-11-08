import subprocess as sp

img = ["nebula2.jpg","newton.jpg"]
for x in img:
    for k in [100,300,500]:
        with open("init.txt","w") as f:
            f.write(f"""../../figs/original/{x}\n3\n../../figs/output/test{x}{k}.ppm\n{k}""")
        sp.run("./a.out")





