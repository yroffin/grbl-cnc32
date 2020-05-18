import { Component, OnInit, ElementRef, ViewChild, AfterViewInit } from '@angular/core';
import {
  Engine, Scene, FreeCamera, Vector3, HemisphericLight, Mesh,
  ArcRotateCamera, AnimationPropertiesOverride, Color3, DirectionalLight, ShadowGenerator, SceneLoader, GizmoManager, AbstractMesh
} from 'babylonjs';
import { Button, Control, AdvancedDynamicTexture, StackPanel } from 'babylonjs-gui';
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

  constructor(private storeService: StoreService) { }

  ngOnInit(): void {
  }

  ngAfterViewInit(): void {
    // Load the 3D engine
    this.engine = new Engine(this.myCanvas.nativeElement, true, { preserveDrawingBuffer: true, stencil: true });
    // Create the scene
    const scene = this.altScene(this.myCanvas.nativeElement);
    // render world
    this.createWorld(scene);
  }

  private createWorld(scene: Scene) {
    // run the render loop
    this.engine.runRenderLoop(() => {
      scene.render();
    });
    // the canvas/window resize event handler
    window.addEventListener('resize', () => {
      this.engine.resize();
    });
  }

  private altScene(element: any) {
    // Model by Mixamo
    this.engine.enableOfflineSupport = false;

    // This is really important to tell js to use decomposeLerp and matrix interpolation
    // Animation.AllowMatricesInterpolation = true;

    const scene = new Scene(this.engine);

    const camera = new ArcRotateCamera('camera1', Math.PI / 2, Math.PI / 4, 3, new Vector3(0, 1, 0), scene);
    camera.attachControl(element, true);

    camera.lowerRadiusLimit = 2;
    camera.upperRadiusLimit = 10;
    camera.wheelDeltaPercentage = 0.01;

    const light = new HemisphericLight('light1', new Vector3(0, 1, 0), scene);
    light.intensity = 0.6;
    light.specular = Color3.Black();

    const light2 = new DirectionalLight('dir01', new Vector3(0, -0.5, -1.0), scene);
    light2.position = new Vector3(0, 5, 5);

    // Shadows
    const shadowGenerator = new ShadowGenerator(1024, light2);
    shadowGenerator.useBlurExponentialShadowMap = true;
    shadowGenerator.blurKernel = 64;

    this.engine.displayLoadingUI();

    SceneLoader.Append('assets/', 'test.stl', scene, (sc) => {
      scene.createDefaultCameraOrLight(true, true, true);
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
    });

    return scene;
  }

}
