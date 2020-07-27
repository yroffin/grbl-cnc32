import { BrowserModule } from '@angular/platform-browser';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { StoreModule } from '@ngrx/store';

import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.main';
import { AppComponent } from './app.component';
import { ConsoleComponent } from './components/console/console.component';
import { HttpClientModule } from '@angular/common/http';

import { InputTextModule } from 'primeng/inputtext';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { InputSwitchModule } from 'primeng/inputswitch';
import { CheckboxModule } from 'primeng/checkbox';
import { TabViewModule } from 'primeng/tabview';
import { ButtonModule } from 'primeng/button';
import { PaperjsComponent } from './components/paperjs/paperjs.component';
import { RadioButtonModule } from 'primeng/radiobutton';
import { ToastModule } from 'primeng/toast';
import { SliderModule } from 'primeng/slider';
import { EffectsModule } from '@ngrx/effects';
import { StoreService, ActionListenerEffects } from './store/store.service';
import { TerminalModule } from 'primeng/terminal';
import { BabylonComponent } from './components/babylon/babylon.component';
import { MessagesModule } from 'primeng/messages';
import { MessageModule } from 'primeng/message';
import { TableModule } from 'primeng/table';
import { MessageService } from 'primeng/api';
import { TooltipModule } from 'primeng/tooltip';
import { CarouselModule } from 'primeng/carousel';

@NgModule({
  declarations: [
    AppComponent,
    ConsoleComponent,
    PaperjsComponent,
    BabylonComponent
  ],
  imports: [
    BrowserModule,
    StoreModule.forRoot(StoreService.reducers),
    EffectsModule.forRoot([ActionListenerEffects]),
    AppRoutingModule,
    BrowserModule,
    AppRoutingModule,
    BrowserAnimationsModule,
    HttpClientModule,
    FormsModule,
    ReactiveFormsModule,
    InputTextModule,
    InputSwitchModule,
    CheckboxModule,
    TabViewModule,
    ButtonModule,
    RadioButtonModule,
    ToastModule,
    SliderModule,
    TerminalModule,
    MessageModule,
    MessagesModule,
    TableModule,
    ToastModule,
    TooltipModule,
    CarouselModule
  ],
  providers: [MessageService],
  bootstrap: [AppComponent]
})
export class AppModule { }
