import { Component, OnInit, ElementRef, ViewChild, AfterViewInit } from '@angular/core';
import {
  Engine, Scene, Vector3, HemisphericLight, Mesh,
  ArcRotateCamera, Color3, DirectionalLight, ShadowGenerator, SceneLoader, GizmoManager, AbstractMesh, VirtualJoysticksCamera
} from 'babylonjs';
import 'babylonjs-loaders';
import * as _ from 'lodash';
import { StoreService } from '../../store/store.service';

@Component({
  selector: 'app-babylon',
  templateUrl: './babylon.component.html',
  styleUrls: ['./babylon.component.css']
})
export class BabylonComponent implements OnInit, AfterViewInit {

  @ViewChild('myCanvas') myCanvas: ElementRef;

  private engine: Engine;
  private scene: Scene;
  private camera: ArcRotateCamera;

  constructor(private storeService: StoreService) { }

  ngOnInit(): void {
  }

  ngAfterViewInit(): void {
    // Load the 3D engine
    this.engine = new Engine(this.myCanvas.nativeElement, true, { preserveDrawingBuffer: true, stencil: true });
    // Create the scene
    this.createScene(this.myCanvas.nativeElement);
    // render world
    this.createWorld();
  }

  async myUploader(event) {
    let fileURL = window.URL.createObjectURL(event.files[0]);
    this.load(fileURL);
  }

  private createWorld() {
    // run the render loop
    this.engine.runRenderLoop(() => {
      this.scene.render();
    });
    // the canvas/window resize event handler
    window.addEventListener('resize', () => {
      this.engine.resize();
    });
  }

  private createScene(element: any): void {
    this.engine.enableOfflineSupport = false;

    // This is really important to tell js to use decomposeLerp and matrix interpolation
    // Animation.AllowMatricesInterpolation = true;

    this.scene = new Scene(this.engine);

    this.camera = new ArcRotateCamera('camera1', 0, 0, 200, new Vector3(0, 0, 0), this.scene, true);
    this.camera.attachControl(element, true);

    const light = new HemisphericLight('light1', new Vector3(0, 1, 0), this.scene);
    light.intensity = 0.6;
    light.specular = Color3.Yellow();

    const light2 = new DirectionalLight('dir01', new Vector3(0, -0.5, -1.0), this.scene);
    light2.position = new Vector3(0, 5, 50);

    // Shadows
    const shadowGenerator = new ShadowGenerator(1024, light2);
    shadowGenerator.useBlurExponentialShadowMap = true;
    shadowGenerator.blurKernel = 64;

    this.engine.displayLoadingUI();
  }

  private load(url: string) {
    const decode = url.split('/');
    SceneLoader.Append(decode[0] + '//' + decode[2] + '/', decode[3], this.scene, (sc) => {

      this.engine.resize();

      console.log(sc.meshes);
      this.camera.focusOn(sc.meshes);
      let min = null;
      let max = null;
      _.each(sc.meshes, (mesh: AbstractMesh) => {
        const boundingBox = mesh.getBoundingInfo().boundingBox;
        if (!min) {
          min = boundingBox.minimumWorld;
          max = boundingBox.maximumWorld;
          return;
        }
        min.MinimizeInPlace(boundingBox.minimumWorld);
        max.MaximizeInPlace(boundingBox.maximumWorld);
      });
      this.storeService.dispatchStlInfo(
        {
          x: min.x,
          y: min.y,
          z: min.z
        },
        {
          x: max.x,
          y: max.y,
          z: max.z
        }
      );
    }, () => {

    }, () => {

    }, '.stl');
  }

}
