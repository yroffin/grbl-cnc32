import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { ConsoleComponent } from 'src/app/components/console/console.component';

const routes: Routes = [
  {
    path: '',
    component: ConsoleComponent
  },
  {
    path: 'console',
    component: ConsoleComponent
  }
];

// Test
@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
