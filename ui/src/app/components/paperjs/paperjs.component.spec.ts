import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { PaperjsComponent } from './paperjs.component';

describe('PaperjsComponent', () => {
  let component: PaperjsComponent;
  let fixture: ComponentFixture<PaperjsComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ PaperjsComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(PaperjsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
