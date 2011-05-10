(Material pinkmat
    (color .9 .5 .5)
    (ka .1) (kd .7) (ks .5) (ksp 20) (ksm 0) (kt 0) (ktn 1)
)

(Material whitemat
    (color .9 .9 .9)
    (ka .1) (kd .8) (ks .1) (ksp 4) (ksm 0) (kt 0) (ktn 1)
)

(Material blackmat
    (color .2 .2 .2)
    (ka .1) (kd .1) (ks .8) (ksp 150) (ksm 0) (kt 0) (ktn 1)
)

(Material voidmat
    (color 0 0 0)
    (ka 0) (kd 0) (ks 0) (ksp 1) (ksm 1) (kt 0) (ktn 1)
)

(Material groundmat
    (color .4 .7 .2)
    (ka .1) (kd .6) (ks .3) (ksp 1) (ksm 1) (kt 0) (ktn 1)
)

(Material skymat
    (color .2 .5 1)
    (ka .1) (kd .8) (ks .1) (ksp .5) (ksm 1) (kt 0) (ktn 1)
)

(Material sunmat
    (color 1 1 0)
    (ka .1) (kd .8) (ks .1) (ksp .5) (ksm 1) (kt 0) (ktn 1)
)

(Sphere wsph (radius 1.0) (material whitemat))
(Sphere psph (radius 1.0) (material pinkmat))
(Sphere bsph (radius 1.0) (material blackmat))
(Sphere vsph (radius 1.0) (material voidmat))
(Sphere usph (radius 1.0) (material groundmat))
(Sphere ssph (radius 1.0) (material skymat))
(Sphere ysph (radius 1.0) (material sunmat))

(Light amb
    (type 3) (color .6 .6 .2)
)

(Light dir
    (type 0) (color 1.0 1.0 .7)
)

(Light pt
    (type 1) (color 1.39 1.2 1.0) (falloff 2) (deaddistance .01)
)

(Light sunlt
    (type 1) (color 1.39 1.2 1.0) (falloff 0) (deaddistance .01)
)

(Camera cam
    (perspective 1)
    (l -.334) (r .334) (b -.334) (t .334) (n 1) # no far plane 
)

(G ear
    (I s1 psph (S .1 1 .05) (R -2.5 1 0 0) (T 0 0 .27) )
    (I s2 wsph (S .3 1.5 .3))
)

(G eye
    (I s3 bsph (S .1 .2 .1))
)

(G face
    (I i1 eye (T -.5 .5 .8))
    (I i2 eye (T  .5 .5 .8))
    (I er1 ear (T -.4 1 0) (R 10 0 0 1))
    (I er2 ear (T  .4 1 0) (R -10 0 0 1))
    (I mouth vsph (S 1.0 .5 .52) (T 0 -.23 .435))
    (I head wsph (S 1.3 1 1))
    (I nose psph (S .1 .05 .05) (T 0 0 1))
)


(G scene
    (I cam1 cam)
    (I dir1 dir (Xform       -.9304  0  .3665 0
                             .1444  -.9191  .3665 0
                             .3369  .3939 .8552 0
                             0  0   0 1)) # points toward (.5,.5,-.5)
    (I amb1 amb)
    (I pt1 pt (T 0 0 -2.5))
    (I f face (T 0 -.62 -5))
    (I floor usph (S 1000 .001 1000) (T 0 -4 0))
    (I sky ssph (S 2000 2000 .001) (T 0 0 -20))
    (I sun ysph (S 3 3 .002) (T -5 5 -20))
    (I sl sunlt (T -5 5 -19))
)

(Render scene)


