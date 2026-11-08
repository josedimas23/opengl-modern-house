// Genera skybox.jpg: un cielo procedural en forma de cubo desplegado en cruz,
// con la misma disposición que usan las coordenadas de textura de dibuja().
// Uso: swift tools/genera_skybox.swift skybox.jpg
import Foundation
import CoreGraphics
import ImageIO
import UniformTypeIdentifiers

typealias V3 = (x: Double, y: Double, z: Double)
func norm(_ v: V3) -> V3 { let l = (v.x*v.x + v.y*v.y + v.z*v.z).squareRoot(); return (v.x/l, v.y/l, v.z/l) }
func mix(_ a: Double, _ b: Double, _ t: Double) -> Double { a + (b - a) * t }
func mix3(_ a: V3, _ b: V3, _ t: Double) -> V3 { (mix(a.x,b.x,t), mix(a.y,b.y,t), mix(a.z,b.z,t)) }
func smooth(_ e0: Double, _ e1: Double, _ x: Double) -> Double { let t = max(0, min(1, (x-e0)/(e1-e0))); return t*t*(3-2*t) }

// ruido de valor 2D y fbm
func hash(_ x: Int, _ y: Int) -> Double {
  var h = UInt64(bitPattern: Int64(x &* 374761393 &+ y &* 668265263))
  h = (h ^ (h >> 13)) &* 1274126177; h ^= h >> 16
  return Double(h & 0xFFFF) / 65535.0
}
func noise(_ x: Double, _ y: Double) -> Double {
  let xi = Int(floor(x)), yi = Int(floor(y)); let xf = x - floor(x), yf = y - floor(y)
  let u = xf*xf*(3-2*xf), v = yf*yf*(3-2*yf)
  return mix(mix(hash(xi,yi), hash(xi+1,yi), u), mix(hash(xi,yi+1), hash(xi+1,yi+1), u), v)
}
func fbm(_ x: Double, _ y: Double, _ oct: Int = 5) -> Double {
  var a = 0.5, f = 1.0, s = 0.0
  for _ in 0..<oct { s += a * noise(x*f, y*f); a *= 0.5; f *= 2.0 }
  return s
}

let sol = norm((0.45, 0.55, 0.70))

// color del entorno visto en la dirección d (y hacia arriba)
func entorno(_ d: V3) -> V3 {
  let azimut = atan2(d.z, d.x)
  // colinas lejanas: altura del horizonte según el azimut (continua en ±pi)
  let colina = 0.03 + 0.09 * fbm(cos(azimut)*3 + 10, sin(azimut)*3 + 10, 4)
  if d.y > colina {
    // cielo: degradado + sol + nubes
    let t = pow(max(0, d.y), 0.5)
    var c = mix3((0.80, 0.88, 0.96), (0.25, 0.50, 0.85), t)
    let cs = d.x*sol.x + d.y*sol.y + d.z*sol.z
    c = mix3(c, (1.0, 0.97, 0.88), smooth(0.995, 0.999, cs) + 0.25 * pow(max(0, cs), 12))
    let px = d.x / max(d.y, 0.05) * 1.6, pz = d.z / max(d.y, 0.05) * 1.6
    let n = fbm(px, pz)
    let nube = smooth(0.52, 0.72, n) * smooth(0.0, 0.25, d.y)
    c = mix3(c, (0.97, 0.97, 0.98), nube * 0.85)
    return c
  } else if d.y > 0 {
    // colinas: verde azulado con bruma hacia la cima
    let h = d.y / colina
    let base: V3 = (0.30, 0.45, 0.33)
    return mix3(base, (0.62, 0.72, 0.78), 0.35 + 0.4 * h)
  } else {
    // suelo: hierba con variación
    let px = d.x / min(d.y, -0.02), pz = d.z / min(d.y, -0.02)
    let n = fbm(px * 3, pz * 3)
    let c: V3 = mix3((0.26, 0.48, 0.20), (0.42, 0.62, 0.28), n)
    return mix3(c, (0.55, 0.68, 0.62), smooth(-0.12, 0.0, d.y) * 0.6)  // bruma en el horizonte
  }
}

// caras del cubo: rectángulo (s0,t0,s1,t1) en la imagen y vértice 3D de cada esquina
// esquinas en orden: (s0,t0) (s1,t0) (s0,t1) (s1,t1); t crece hacia abajo en la imagen
let v: [V3] = [(-1,-1,1),(1,-1,1),(1,1,1),(-1,1,1),(-1,-1,-1),(1,-1,-1),(1,1,-1),(-1,1,-1)]
let caras: [(Double,Double,Double,Double,[Int])] = [
  (0.25, 0.50, 0.50, 0.75, [4,5,0,1]),  // abajo
  (0.25, 0.00, 0.50, 0.25, [7,6,3,2]),  // arriba
  (0.25, 0.25, 0.50, 0.50, [3,2,0,1]),  // delante
  (0.75, 0.25, 1.00, 0.50, [6,7,5,4]),  // atrás
  (0.00, 0.25, 0.25, 0.50, [7,3,4,0]),  // izquierda
  (0.50, 0.25, 0.75, 0.50, [2,6,1,5]),  // derecha
]

let N = 1024
var px = [UInt8](repeating: 0, count: N*N*4)
for y in 0..<N { for x in 0..<N {
  let s = (Double(x)+0.5)/Double(N), t = (Double(y)+0.5)/Double(N)
  var col: V3 = (0,0,0)
  for (s0,t0,s1,t1,c) in caras where s >= s0 && s < s1 && t >= t0 && t < t1 {
    let a = (s-s0)/(s1-s0), b = (t-t0)/(t1-t0)
    let p = mix3(mix3(v[c[0]], v[c[1]], a), mix3(v[c[2]], v[c[3]], a), b)
    col = entorno(norm(p))
  }
  let i = (y*N + x)*4
  px[i] = UInt8(max(0,min(255,col.x*255))); px[i+1] = UInt8(max(0,min(255,col.y*255))); px[i+2] = UInt8(max(0,min(255,col.z*255))); px[i+3] = 255
}}

let ctx = CGContext(data: &px, width: N, height: N, bitsPerComponent: 8, bytesPerRow: N*4,
                    space: CGColorSpace(name: CGColorSpace.sRGB)!, bitmapInfo: CGImageAlphaInfo.noneSkipLast.rawValue)!
let img = ctx.makeImage()!
let url = URL(fileURLWithPath: CommandLine.arguments.count > 1 ? CommandLine.arguments[1] : "skybox.jpg")
let dst = CGImageDestinationCreateWithURL(url as CFURL, UTType.jpeg.identifier as CFString, 1, nil)!
CGImageDestinationAddImage(dst, img, [kCGImageDestinationLossyCompressionQuality: 0.9] as CFDictionary)
CGImageDestinationFinalize(dst)
