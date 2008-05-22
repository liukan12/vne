% generate a shape for import to VNE
% create it as a levelset f(x,y,z) = 0

[xx,yy,zz] = meshgrid(-1:.25:1,-1:.25:1,-1:.25:1);

fxyz = xx.^2+yy.^2+xx.*zz.^2 - 0.2;

% get faces and vertices
[f,v] = isosurface(xx,yy,zz,fxyz,0.0); 
[nf,nv] = reducepatch(f,v,100);
n = isonormals(xx,yy,zz,fxyz,nv)

trisurf(nf,nv(:,1),nv(:,2),nv(:,3))

csvwrite('faces5.dat',nf);
csvwrite('verts5.dat',nv);
csvwrite('norms5.dat',n);